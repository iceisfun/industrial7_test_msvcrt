/* TC
 * id:    ucr_neg_stdio_vfprintf_null_stream
 * area:  ucrt_only
 * funcs: fopen fclose remove
 * procs: __stdio_common_vfprintf __stdio_common_vfwprintf
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs fprintf, fwprintf (NULL stream or format -> invalid parameter handler, EINVAL, -1)
 * note:  temp file tc_ucr_neg_stdio_vfprintf_null_stream_*.tmp
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vf_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef int (__cdecl *vfw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vf_t p_vf; static vfw_t p_vfw;
#define TMP "tc_ucr_neg_stdio_vfprintf_null_stream_a.tmp"
static int callf(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vf(0, f, fmt, NULL, ap); va_end(ap); return r; }
static int callfw(FILE *f, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vfw(0, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(narrow_null_stream)
{
    int r;
    if (!p_vf) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = callf(NULL, "x%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wide_null_stream)
{
    int r;
    if (!p_vfw) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = callfw(NULL, L"x%d", 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    FILE *f; int r;
    if (!p_vf) TC_SKIP("not exported");
    f = fopen(TMP, "wb"); CHECK(f != NULL); if (!f) return;
    errno = 0; tc_iph_count = 0;
    r = callf(f, NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    fclose(f); remove(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vf = (vf_t)tc_crt_proc("__stdio_common_vfprintf");
    p_vfw = (vfw_t)tc_crt_proc("__stdio_common_vfwprintf");
    TC_RUN(narrow_null_stream); TC_RUN(wide_null_stream); TC_RUN(null_format);
    return tc_end();
}
