/* TC
 * id:    pfl_neg_swprintf_l_null_fmt
 * area:  stdio_printf
 * funcs: _swprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs swprintf, _swprintf_l ("Parameter validation": NULL format -> EINVAL, -1)
 * note:  ucrt signature _swprintf_l(buf, fmt, locale, ...) (no count; the legacy CRTs differ, hence >=140 only).
 *        The return value is asserted only as negative (docs: negative on error); the unbounded wide form is not documented to return exactly -1.
 *        NULL destination buffer is a separate probe: pfl_neg_swprintf_l_null_buf_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>
#include <stdarg.h>

/* ucrt headers in the build toolchain do not declare _swprintf_l; the Windows SDK inlines it as below
 * (unbounded count, legacy wide specifiers option 4). */
static int swl(wchar_t *b, const wchar_t *f, _locale_t l, ...)
{
    va_list ap; int r; va_start(ap, l);
    r = __stdio_common_vswprintf(4ULL, b, (size_t)-1, f, l, ap);
    va_end(ap); return r;
}

TC_CASE(null_fmt)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[8]; const wchar_t *volatile f = NULL; int r;
    CHECK(l != NULL); if (!l) return;
    b[0] = L'Q'; tc_iph_count = 0; errno = 0;
    r = swl(b, f, l, 1);
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    _free_locale(l);
}
TC_CASE(valid_after_failure)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t b[16]; int r;
    CHECK(l != NULL); if (!l) return;
    tc_iph_count = 0;
    r = swl(b, L"%d|%ls", l, 12, L"ab");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"12|ab"); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_fmt); TC_RUN(valid_after_failure);
    return tc_end();
}
