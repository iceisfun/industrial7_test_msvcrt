/* TC
 * id:    ucr_neg_stdio_vsprintf_percent_n_disabled
 * area:  ucrt_only
 * funcs: fopen fclose remove
 * procs: __stdio_common_vsprintf __stdio_common_vfprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _set_printf_count_output (%n support disabled by default), printf format specification (%n)
 * note:  temp file tc_ucr_neg_stdio_vsprintf_percent_n_disabled_*.tmp
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
static vs_t p_vs; static vf_t p_vf;
#define TMP "tc_ucr_neg_stdio_vsprintf_percent_n_disabled_a.tmp"
static int calls(char *b, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vs(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(sprintf_n)
{
    char b[64]; volatile int target = 0x5a5a; int r;
    if (!p_vs) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = calls(b, "abc%n", (int *)&target);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(target, 0x5a5a);
}
TC_CASE(fprintf_n)
{
    FILE *f; volatile int target = 0x5a5a; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    r = callf(f, "abc%n", (int *)&target);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(target, 0x5a5a);
    fclose(f); remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    TC_RUN(sprintf_n); TC_RUN(fprintf_n);
    return tc_end();
}
