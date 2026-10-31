/* TC
 * id:    pfo_neg_vsprintf_trunc_win7
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsnprintf
 * procs: _vsnprintf_s _vsnprintf_c
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf, _vsnprintf_s, _vsnprintf_l ("if len > count ... returns a negative value"; _TRUNCATE -> -1, no handler); _snprintf_c
 * note:  Win7-era CRT overflow paths. Only the negative return, handler count and "nothing written past count" are asserted.
 *        NUL termination of _vsnprintf_c and the NULL/count-0 case live in pfo_probe_vsnprintf_c_nul_win7 / pfo_neg_probe_vsnprintf_null_count0_win7.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define GUARD 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = GUARD; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != GUARD) return 0; return 1; }

typedef int (__cdecl *vs_t)(char *, size_t, size_t, const char *, va_list);
typedef int (__cdecl *vc_t)(char *, size_t, const char *, va_list);
static vs_t p_vs;
static vc_t p_vc;

static int call_vsnprintf(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r; }
static int call_vs(char *b, size_t sz, size_t cnt, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_vs(b, sz, cnt, f, ap); va_end(ap); return r; }
static int call_vc(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_vc(b, n, f, ap); va_end(ap); return r; }

TC_CASE(vsnprintf_overflow)
{
    char b[32]; int r;
    fill(b, 32); tc_iph_count = 0;
    r = call_vsnprintf(b, 5, "%s", "abcdefghij");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_MEM(b, "abcde", 5);
    CHECK(guard_ok(b, 5, 32));
}
TC_CASE(vsnprintf_exact_fit)
{
    char b[32]; int r;
    fill(b, 32); tc_iph_count = 0;
    r = call_vsnprintf(b, 5, "%s", "abcde");   /* exactly count chars: returns count, no NUL required */
    CHECK_EQ_I(r, 5);
    CHECK_MEM(b, "abcde", 5);
    CHECK(guard_ok(b, 5, 32));
}
TC_CASE(vsnprintf_s_truncate)
{
    char b[32]; int r;
    if (!p_vs) TC_SKIP("_vsnprintf_s not exported");
    fill(b, 32); tc_iph_count = 0;
    r = call_vs(b, 5, (size_t)-1 /* _TRUNCATE */, "%s", "abcdefghij");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(b, "abcd");
    CHECK(guard_ok(b, 5, 32));
}
TC_CASE(vsnprintf_s_null_buffer)
{
    int r;
    if (!p_vs) TC_SKIP("_vsnprintf_s not exported");
    tc_iph_count = 0;
    r = call_vs(NULL, 10, 5, "%d", 1);
    CHECK(r < 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(vsnprintf_c_overflow)
{
    char b[32]; int r;
    if (!p_vc) TC_SKIP("_vsnprintf_c not exported");
    fill(b, 32); tc_iph_count = 0;
    r = call_vc(b, 5, "%s", "abcdefghij");
    (void)r;   /* return value of _vsnprintf_c on overflow lives in pfo_probe_vsnprintf_c_nul_win7 */
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK(guard_ok(b, 5, 32));
}

int main(void)
{
    tc_begin();
    p_vs = (vs_t)tc_crt_proc("_vsnprintf_s");
    p_vc = (vc_t)tc_crt_proc("_vsnprintf_c");
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(vsnprintf_overflow); TC_RUN(vsnprintf_exact_fit); TC_RUN(vsnprintf_s_truncate);
    TC_RUN(vsnprintf_s_null_buffer); TC_RUN(vsnprintf_c_overflow);
    return tc_end();
}
