/* TC
 * id:    ucr_neg_stdio_vsscanf_null_args
 * area:  ucrt_only
 * kind:  negative
 * funcs: __stdio_common_vsscanf __stdio_common_vswscanf
 * crt:   win10
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sscanf/swscanf (invalid parameter handler; returns EOF and sets errno to EINVAL when input or format is NULL); Parameter Validation
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <stddef.h>
#include <errno.h>

static int vsc(unsigned long long opts, const char *in, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsscanf(opts, in, (size_t)-1, f, NULL, ap); va_end(ap);
    return r;
}

static int vsw(unsigned long long opts, const wchar_t *in, const wchar_t *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vswscanf(opts, in, (size_t)-1, f, NULL, ap); va_end(ap);
    return r;
}
TC_CASE(null_input) { int v = 5; int r; tc_iph_count = 0; errno = 0; r = vsc(0, NULL, "%d", &v);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(v, 5); }
TC_CASE(null_format) { int v = 5; int r; tc_iph_count = 0; errno = 0; r = vsc(0, "1", NULL, &v);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(v, 5); }
TC_CASE(wide_null_input) { int v = 5; int r; tc_iph_count = 0; errno = 0; r = vsw(0, NULL, L"%d", &v);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(v, 5); }
TC_CASE(wide_null_format) { int v = 5; int r; tc_iph_count = 0; errno = 0; r = vsw(0, L"1", NULL, &v);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(v, 5); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_input);
    TC_RUN(null_format);
    TC_RUN(wide_null_input);
    TC_RUN(wide_null_format);
    return tc_end();
}
