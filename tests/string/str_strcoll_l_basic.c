/* TC
 * id:    str_strcoll_l_basic
 * area:  string
 * funcs: _strcoll_l _stricoll_l _strncoll_l _strnicoll_l _create_locale _free_locale
 * procs: 
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs strcoll_l family, _create_locale
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <locale.h>
static int sgn(int v) { return (v > 0) - (v < 0); }
static _locale_t L;

TC_CASE(strcoll_l)
{
    volatile const char *a = "abc", *b = "abd", *c = "abc";
    if (!L) TC_SKIP("no C locale");
    CHECK_EQ_I(sgn(_strcoll_l((const char *)a, (const char *)b, L)), -1);
    CHECK_EQ_I(sgn(_strcoll_l((const char *)b, (const char *)a, L)), 1);
    CHECK_EQ_I(_strcoll_l((const char *)a, (const char *)c, L), 0);
}
TC_CASE(stricoll_l)
{
    volatile const char *a = "AbC", *b = "aBc", *c = "abd";
    if (!L) TC_SKIP("no C locale");
    CHECK_EQ_I(_stricoll_l((const char *)a, (const char *)b, L), 0);
    CHECK_EQ_I(sgn(_stricoll_l((const char *)a, (const char *)c, L)), -1);
}
TC_CASE(strncoll_l)
{
    volatile const char *a = "abcX", *b = "abcY";
    if (!L) TC_SKIP("no C locale");
    CHECK_EQ_I(_strncoll_l((const char *)a, (const char *)b, 3, L), 0);
    CHECK_EQ_I(sgn(_strncoll_l((const char *)a, (const char *)b, 4, L)), -1);
}
TC_CASE(strnicoll_l)
{
    volatile const char *a = "ABCx", *b = "abcY";
    if (!L) TC_SKIP("no C locale");
    CHECK_EQ_I(_strnicoll_l((const char *)a, (const char *)b, 3, L), 0);
    CHECK_EQ_I(sgn(_strnicoll_l((const char *)a, (const char *)b, 4, L)), -1);
}
int main(void)
{
    tc_begin(); L = _create_locale(LC_ALL, "C");
    CHECK(L); TC_RUN(strcoll_l); TC_RUN(stricoll_l); TC_RUN(strncoll_l); TC_RUN(strnicoll_l);
    if (L) _free_locale(L);
    return tc_end();
}
