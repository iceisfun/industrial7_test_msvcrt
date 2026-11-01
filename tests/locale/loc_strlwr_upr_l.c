/* TC
 * id:    loc_strlwr_upr_l
 * area:  locale
 * funcs: _strlwr_l _strupr_l _wcslwr_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _strlwr_l, _strupr_l, _wcslwr_l
 * note:  Converts in place and returns its argument. Skips 1252 cases if the locale is unavailable.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>
/* mingw headers do not declare these; without prototypes the returned pointer is truncated to int (test defect) */
char *__cdecl _strlwr_l(char *, _locale_t);
char *__cdecl _strupr_l(char *, _locale_t);
wchar_t *__cdecl _wcslwr_l(wchar_t *, _locale_t);

static _locale_t l1252, lc;

TC_CASE(lwr_1252)
{
    char b[8]; strcpy(b, "\xC9" "ABC");
    if (!l1252) TC_SKIP("locale unavailable");
    _strlwr_l(b, l1252); CHECK_STR(b, "\xE9" "abc");
}
TC_CASE(upr_1252)
{
    char b[8]; strcpy(b, "\xE9" "abc");
    if (!l1252) TC_SKIP("locale unavailable");
    _strupr_l(b, l1252); CHECK_STR(b, "\xC9" "ABC");
}
TC_CASE(c_locale_ascii_only)
{
    char b[8], c[8]; strcpy(b, "\xC9" "ABC"); strcpy(c, "\xE9" "abc");
    _strlwr_l(b, lc); CHECK_STR(b, "\xC9" "abc");
    _strupr_l(c, lc); CHECK_STR(c, "\xE9" "ABC");
}
TC_CASE(wcslwr_l_unicode)
{
    wchar_t b[4];
    if (!l1252) TC_SKIP("locale unavailable");
    b[0] = 0xC9; b[1] = 0xC0; b[2] = L'Z'; b[3] = 0;
    _wcslwr_l(b, l1252);
    CHECK_EQ_I(b[0], 0xE9); CHECK_EQ_I(b[1], 0xE0); CHECK_EQ_I(b[2], L'z');
}
TC_CASE(returns_same_pointer)
{
    char b[8]; wchar_t w[4];
    strcpy(b, "ABC"); w[0] = L'A'; w[1] = 0;
    CHECK_EQ_P(_strlwr_l(b, lc), b);
    CHECK_EQ_P(_strupr_l(b, lc), b);
    CHECK_EQ_P(_wcslwr_l(w, lc), w);
}
int main(void)
{
    tc_begin();
    l1252 = _create_locale(LC_ALL, "English_United States.1252");
    lc = _create_locale(LC_ALL, "C");
    if (!lc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(lwr_1252); TC_RUN(upr_1252); TC_RUN(c_locale_ascii_only); TC_RUN(wcslwr_l_unicode); TC_RUN(returns_same_pointer);
    if (l1252) _free_locale(l1252);
    _free_locale(lc);
    return tc_end();
}
