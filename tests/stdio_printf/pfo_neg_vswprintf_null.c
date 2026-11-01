/* TC
 * id:    pfo_neg_vswprintf_null
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vswprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs swprintf, _snwprintf, "Parameter validation", printf type field characters
 * note:  Direct calls with options 0. The control case uses _CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR (0x2): no handler, returns needed length.
 *        Win10 22H2 fixes: the control passed a wide string to a plain %s, which in the ISO mode of the raw worker is a narrow string (got r=1, "a"): now %ls.
 *        invalid_conversion: the plain worker does not validate ("%y" -> "y", 1, no handler) - observed.
 *        Raw evidence (baseline): invalid_conversion r == -1 got=1 ; tc_iph_count == 1 got=0 ; control_standard_truncation r == 8 got=1, b == L"abc" mismatch.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <stdarg.h>
#include <errno.h>

static int vsw(unsigned long long opt, wchar_t *b, size_t n, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = __stdio_common_vswprintf(opt, b, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(null_format)
{
    wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vsw(0, b, 8, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer_nonzero_count)
{
    wchar_t *volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = vsw(0, b, 8, L"abc");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(invalid_conversion)
{
    wchar_t b[8]; const wchar_t *volatile f = L"%y"; int r;
    tc_iph_count = 0; errno = 0;
    r = vsw(0, b, 8, f, 1);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(tc_iph_count, 0); CHECK_WSTR(b, L"y");   /* observed: plain (non-_s) worker does not validate; '%' dropped */
}
TC_CASE(control_standard_truncation)
{
    wchar_t b[4]; int r;
    tc_iph_count = 0;
    r = vsw(2, b, 4, L"%ls", L"abcdefgh");   /* %ls: with options lacking LEGACY_WIDE_SPECIFIERS a plain %s is a NARROW string in wide printf (ISO) */
    CHECK_EQ_I(r, 8);
    CHECK_WSTR(b, L"abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_format); TC_RUN(null_buffer_nonzero_count); TC_RUN(invalid_conversion); TC_RUN(control_standard_truncation);
    return tc_end();
}
