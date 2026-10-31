/* TC
 * id:    exp_neg_create_locale_bad_name
 * area:  exports
 * funcs:
 * procs: _create_locale _free_locale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120 ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale: returns NULL if the locale cannot be created
 */
#include "tc.h"
#include <locale.h>

typedef void *(__cdecl *create_t)(int, const char *);
static create_t p_create;
static const char *volatile bogus = "xx_NONEXISTENT_zz";

TC_CASE(bogus_name_null)
{
    if (!p_create) TC_SKIP("_create_locale not exported");
    tc_iph_install();
    CHECK(p_create(LC_ALL, bogus) == NULL);
    CHECK(p_create(LC_CTYPE, bogus) == NULL);
}
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale");
    TC_RUN(bogus_name_null);
    return tc_end();
}
