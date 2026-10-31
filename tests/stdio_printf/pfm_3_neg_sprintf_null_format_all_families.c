/* TC
 * id:    pfm_3_neg_sprintf_null_format_all_families
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s printf_s fprintf_s vsprintf_s swprintf_s _snprintf_s
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, printf_s, fprintf_s, vsprintf_s, swprintf_s, _snprintf_s (format == NULL: invalid parameter handler, return -1, errno EINVAL)
 * note:  On ucrtbase these are SDK inlines over __stdio_common_v*_s; on older CRTs they are called through the DLL export (mingw's import
 *        libraries lack them). printf_s runs in a child with stdout redirected to a temp file. The state of buffer[0] after failure is
 *        probed separately in pfm_3_neg_sprintf_s_null_format_buf_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

#define OUT "tc_pfm_3_nf.tmp"

#if TC_CRT_VER >= 140
#define P(n) n
#else
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
TC_DECL_PROC(int, printf_s, (const char *, ...));
TC_DECL_PROC(int, fprintf_s, (FILE *, const char *, ...));
TC_DECL_PROC(int, vsprintf_s, (char *, size_t, const char *, va_list));
TC_DECL_PROC(int, swprintf_s, (wchar_t *, size_t, const wchar_t *, ...));
TC_DECL_PROC(int, _snprintf_s, (char *, size_t, size_t, const char *, ...));
#define P(n) tc_p_##n
#endif

static const char *volatile nf = NULL;
static const wchar_t *volatile nwf = NULL;

static int call_vs(char *b, size_t n, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = P(vsprintf_s)(b, n, f, ap); va_end(ap); return r; }

#define BEGIN tc_iph_count = 0; errno = 0
#define EXPECT_FAIL(r) do { CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(sprintf_s_null)  { char b[16]; int r; BEGIN; r = P(sprintf_s)(b, sizeof b, nf, 1); EXPECT_FAIL(r); }
TC_CASE(vsprintf_s_null) { char b[16]; int r; BEGIN; r = call_vs(b, sizeof b, nf, 1); EXPECT_FAIL(r); }
TC_CASE(swprintf_s_null) { wchar_t b[16]; int r; BEGIN; r = P(swprintf_s)(b, 16, nwf, 1); EXPECT_FAIL(r); }
TC_CASE(snprintf_s_null) { char b[16]; int r; BEGIN; r = P(_snprintf_s)(b, sizeof b, 8, nf, 1); EXPECT_FAIL(r); }
TC_CASE(snprintf_s_null_truncate) { char b[16]; int r; BEGIN; r = P(_snprintf_s)(b, sizeof b, _TRUNCATE, nf, 1); EXPECT_FAIL(r); }
TC_CASE(fprintf_s_null)
{
    FILE *f = fopen(OUT, "w"); int r;
    CHECK(f != NULL); if (!f) return;
    BEGIN; r = P(fprintf_s)(f, nf, 1);
    EXPECT_FAIL(r);
    fclose(f);
}
TC_CASE(printf_s_null_child)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("printf_s_null", &code, 20000));
    CHECK_EQ_U(code, 0);
}
static int child_main(const char *n)
{
    int r, bad = 0; (void)n;
    if (!freopen(OUT, "w", stdout)) return 70;
    tc_iph_install();
    BEGIN; r = P(printf_s)(nf, 1);
    if (r != -1) bad |= 1;
    if (errno != EINVAL) bad |= 2;
    if (tc_iph_count != 1) bad |= 4;
    return bad;
}
int main(void)
{
    const char *c;
    tc_begin();
#if TC_CRT_VER < 140
    TC_LOAD_PROC(sprintf_s); TC_LOAD_PROC(printf_s); TC_LOAD_PROC(fprintf_s); TC_LOAD_PROC(vsprintf_s); TC_LOAD_PROC(swprintf_s); TC_LOAD_PROC(_snprintf_s);
    if (!tc_p_sprintf_s || !tc_p_printf_s || !tc_p_fprintf_s || !tc_p_vsprintf_s || !tc_p_swprintf_s || !tc_p__snprintf_s) {
        tc_write("TC FAIL " TC_TEST_ID "::__missing _s export\n"); return 2;
    }
#endif
    if ((c = tc_child_name()) != NULL) return child_main(c);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_s_null); TC_RUN(vsprintf_s_null); TC_RUN(swprintf_s_null); TC_RUN(snprintf_s_null);
    TC_RUN(snprintf_s_null_truncate); TC_RUN(fprintf_s_null); TC_RUN(printf_s_null_child);
    remove(OUT);
    return tc_end();
}
