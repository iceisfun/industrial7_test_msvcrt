/* TC
 * id:    ucr_neg_stdio_vsprintf_s_n_wide_and_file
 * area:  ucrt_only
 * funcs: fopen fclose remove _set_printf_count_output _get_printf_count_output
 * procs: __stdio_common_vswprintf_s __stdio_common_vfprintf __stdio_common_vfwprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_printf_count_output (%n disabled by default), printf format specification (%n)
 * note:  temp file tc_ucr_neg_stdio_vsprintf_s_n_wide_and_file_*.tmp. The enabled case restores the setting to 0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vws_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef int (__cdecl *vfw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vws_t p_vwss; static vf_t p_vf; static vfw_t p_vfw;
#define TMP "tc_ucr_neg_stdio_vsprintf_s_n_wide_and_file_a.tmp"
static int callws(wchar_t *b, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vwss(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int callfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(wide_disabled)
{
    wchar_t b[64]; volatile int t = 0x5a5a; int r;
    if (!p_vwss) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = callws(b, L"ab%n", (int *)&t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(t, 0x5a5a);
}
TC_CASE(file_disabled)
{
    FILE *f; volatile int t = 0x5a5a; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    r = callf(f, "ab%n", (int *)&t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(t, 0x5a5a);
    fclose(f); remove(TMP);
}
TC_CASE(filew_disabled)
{
    FILE *f; volatile int t = 0x5a5a; int r;
    if (!p_vfw) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    r = callfw(f, L"ab%n", (int *)&t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(t, 0x5a5a);
    fclose(f); remove(TMP);
}
TC_CASE(enabled_all)
{
    wchar_t b[64]; FILE *f; volatile int t = -1; int r;
    if (!p_vwss || !p_vf || !p_vfw) TC_SKIP("not exported");
    CHECK_EQ_I(_set_printf_count_output(1), 0);
    tc_iph_count = 0;
    r = callws(b, L"ab%ncd", (int *)&t);
    CHECK_EQ_I(r, 4); CHECK_EQ_I(t, 2); CHECK_WSTR(b, L"abcd");
    f = fopen(TMP, "wb"); CHECK(f != NULL);
    if (f) {
        t = -1; r = callf(f, "abc%nd", (int *)&t); CHECK_EQ_I(r, 4); CHECK_EQ_I(t, 3);
        t = -1; r = callfw(f, L"a%nbcd", (int *)&t); CHECK_EQ_I(r, 4); CHECK_EQ_I(t, 1);
        fclose(f);
    }
    remove(TMP);
    _set_printf_count_output(0);
    CHECK_EQ_I(_get_printf_count_output(), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vwss = (vws_t)tc_crt_proc("__stdio_common_vswprintf_s");
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    p_vfw = (vfw_t)tc_crt_proc("__stdio_common_vfwprintf");
    TC_RUN(wide_disabled); TC_RUN(file_disabled); TC_RUN(filew_disabled); TC_RUN(enabled_all);
    return tc_end();
}
