/* TC
 * id:    ct_neg_setlocale_bogus_name
 * area:  ctype
 * funcs: setlocale ___mb_cur_max_func toupper
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale (returns NULL if the locale cannot be honored; current locale unchanged)
 * note:  C11 7.11.1.1.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

TC_CASE(bogus_name_returns_null)
{
    volatile const char *n = "No_Such_Locale_xyz.99999";
    CHECK(setlocale(LC_ALL, (const char *)n) == NULL);
}
TC_CASE(locale_unchanged_after_failure)
{
    const char *q; volatile int c = 0xE9;
    setlocale(LC_ALL, "No_Such_Locale_xyz.99999");
    q = setlocale(LC_CTYPE, NULL);
    CHECK(q != NULL); if (q) CHECK_STR(q, "C");
    CHECK_EQ_I(___mb_cur_max_func(), 1);
    CHECK_EQ_I(toupper(c), 0xE9);
}
int main(void) { tc_begin(); TC_RUN(bogus_name_returns_null); TC_RUN(locale_unchanged_after_failure); return tc_end(); }
