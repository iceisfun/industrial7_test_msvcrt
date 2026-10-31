/* TC
 * id:    pfl_neg_swprintf_l_null_buf_probe
 * area:  stdio_printf
 * funcs: _swprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs swprintf ("Parameter validation")
 * note:  PROBE: ucrt _swprintf_l(NULL, L"x", loc) (unbounded count, NULL buffer) is rejected: invalid parameter handler once, -1, EINVAL.
 *        Wide twin of pfl_neg_sprintf_l_null_buf_probe; not spelled out for the unsafe variants in the docs.
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

TC_CASE(null_buf)
{
    _locale_t l = _create_locale(LC_ALL, "C"); wchar_t *volatile b = NULL; int r;
    CHECK(l != NULL); if (!l) return;
    tc_iph_count = 0; errno = 0;
    r = swl(b, L"x", l);
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_buf);
    return tc_end();
}
