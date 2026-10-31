/* TC
 * id:    pfu_sprintf_s_l_basic
 * area:  stdio_printf
 * funcs: _sprintf_s_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l; _create_locale
 * note:  NULL locale pointer behaviour is covered elsewhere (pfl_probe_sprintf_l_null_locale); here the current C locale is used via _create_locale("C") and via the plain-C-locale default.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_CASE(explicit_c_locale)
{
    char b[32]; volatile double d = 3.14159; int n; _locale_t loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    if (!loc) return;
    n = _sprintf_s_l(b, sizeof b, "%.2f", loc, d);
    CHECK_EQ_I(n, 4); CHECK_STR(b, "3.14");
    _free_locale(loc);
}
TC_CASE(exact_fit)
{
    char b[5]; volatile int v = 1234; int n; _locale_t loc = _create_locale(LC_ALL, "C");
    if (!loc) { CHECK(loc != NULL); return; }
    n = _sprintf_s_l(b, 5, "%d", loc, v);
    CHECK_EQ_I(n, 4); CHECK_STR(b, "1234");
    _free_locale(loc);
}
TC_CASE(same_as_sprintf_s)
{
    char a[64], c[64]; volatile double d = 2.5; volatile int v = -7; int n1, n2; _locale_t loc = _create_locale(LC_ALL, "C");
    if (!loc) { CHECK(loc != NULL); return; }
    n1 = sprintf_s(a, sizeof a, "%d:%8.3f:%s", v, d, "x");
    n2 = _sprintf_s_l(c, sizeof c, "%d:%8.3f:%s", loc, v, d, "x");
    CHECK_EQ_I(n1, n2); CHECK_STR(a, c); CHECK_STR(a, "-7:   2.500:x");
    _free_locale(loc);
}
int main(void)
{
    tc_begin();
    TC_RUN(explicit_c_locale); TC_RUN(exact_fit); TC_RUN(same_as_sprintf_s);
    return tc_end();
}
