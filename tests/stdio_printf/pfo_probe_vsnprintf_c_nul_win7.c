/* TC
 * id:    pfo_probe_vsnprintf_c_nul_win7
 * area:  stdio_printf
 * kind:  positive
 * funcs:
 * procs: _vsnprintf_c
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf_c, _vsnprintf_c
 * note:  PROBE: on overflow _vsnprintf_c returns a negative value, and on overflow _vsnprintf_c leaves the buffer NUL-terminated, and an exactly-fitting
 *        string (count-1 chars) is returned with its length. Exact truncated contents are not asserted beyond a NUL within count bytes.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vc_t)(char *, size_t, const char *, va_list);
static vc_t p_vc;
static int call_vc(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_vc(b, n, f, ap); va_end(ap); return r; }

TC_CASE(overflow_nul)
{
    char b[32]; int i, nul = 0;
    if (!p_vc) TC_SKIP("_vsnprintf_c not exported");
    for (i = 0; i < 32; i++) b[i] = 0x5A;
    call_vc(b, 5, "%s", "abcdefghij");
    for (i = 0; i < 5; i++) if (b[i] == 0) nul = 1;
    CHECK(nul);
    CHECK_EQ_I((unsigned char)b[5], 0x5A);
}
TC_CASE(overflow_ret)
{
    char b[32]; int r;
    if (!p_vc) TC_SKIP("_vsnprintf_c not exported");
    r = call_vc(b, 5, "%s", "abcdefghij");
    CHECK(r < 0);
}
TC_CASE(short_fits)
{
    char b[32]; int r;
    if (!p_vc) TC_SKIP("_vsnprintf_c not exported");
    r = call_vc(b, 8, "%s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    p_vc = (vc_t)tc_crt_proc("_vsnprintf_c");
    tc_iph_install();
    TC_RUN(overflow_nul); TC_RUN(overflow_ret); TC_RUN(short_fits);
    return tc_end();
}
