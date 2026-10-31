/* TC
 * id:    str_stricmp_l_basic
 * area:  string
 * funcs: _stricmp_l _strnicmp_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _stricmp_l, _strnicmp_l, _create_locale, _free_locale
 */
#include "tc.h"
#include <string.h>
#include <locale.h>

static volatile const char *pa, *pb;
static _locale_t loc;

TC_CASE(created) { CHECK(loc != NULL); }
TC_CASE(fold_ascii)
{
    if (!loc) TC_SKIP("no C locale");
    pa = "Hello"; pb = "hELLO"; CHECK_EQ_I(_stricmp_l((const char *)pa, (const char *)pb, loc), 0);
    pa = "abc"; pb = "ABD"; CHECK(_stricmp_l((const char *)pa, (const char *)pb, loc) < 0);
    pa = "b"; pb = "A"; CHECK(_stricmp_l((const char *)pa, (const char *)pb, loc) > 0);
}
TC_CASE(count)
{
    if (!loc) TC_SKIP("no C locale");
    pa = "ABCx"; pb = "abcy";
    CHECK_EQ_I(_strnicmp_l((const char *)pa, (const char *)pb, 3, loc), 0);
    CHECK(_strnicmp_l((const char *)pa, (const char *)pb, 4, loc) < 0);
    CHECK_EQ_I(_strnicmp_l((const char *)pa, (const char *)pb, 0, loc), 0);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    TC_RUN(created); TC_RUN(fold_ascii); TC_RUN(count);
    if (loc) _free_locale(loc);
    return tc_end();
}
