/* TC
 * id:    str_strxfrm_l_basic
 * area:  string
 * funcs: _strxfrm_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs strxfrm, _strxfrm_l (same as strxfrm but uses the locale passed)
 */
#include "tc.h"
#include <string.h>
#include <locale.h>

static volatile const char *vs;

TC_CASE(c_locale)
{
    char d[32]; _locale_t loc = _create_locale(LC_ALL, "C"); vs = "Hello";
    CHECK(loc != NULL);
    memset(d, 'x', sizeof d);
    CHECK_EQ_U(_strxfrm_l(d, (const char *)vs, sizeof d, loc), 5);
    CHECK_STR(d, "Hello");
    CHECK_EQ_U(_strxfrm_l(NULL, (const char *)vs, 0, loc), 5);
    if (loc) _free_locale(loc);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale);
    return tc_end();
}
