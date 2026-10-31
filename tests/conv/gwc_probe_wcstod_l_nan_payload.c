/* TC
 * id:    gwc_probe_wcstod_l_nan_payload
 * area:  conv
 * funcs: _wcstod_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.22.1.3 (nan(n-char-sequence)); MS docs strtod
 * note:  Probing that UCRT _wcstod_l accepts "nan(abc)" and consumes the parenthesised sequence (endptr after ')').
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static const wchar_t *volatile vp;
static double D(const wchar_t *s, wchar_t **e) { vp = s; return _wcstod_l(vp, e, L); }

TC_CASE(nan_payload) { const wchar_t *a = L"nan(abc)x"; wchar_t *e; CHECK_IS_NAN(D(a, &e)); CHECK_EQ_P(e, a + 8); }
int main(void)
{
    tc_begin();
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(nan_payload);
    _free_locale(L);
    return tc_end();
}
