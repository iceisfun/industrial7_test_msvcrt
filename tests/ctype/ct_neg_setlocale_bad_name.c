/* TC
 * id:    ct_neg_setlocale_bad_name
 * area:  ctype
 * funcs: setlocale
 * procs: _create_locale _free_locale
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale: returns NULL if the locale cannot be honoured, locale unchanged; C11 7.11.1.1
 * note:  Bad locale names fail; global locale stays C. _create_locale(bad) returns NULL (checked only where exported).
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static const char *volatile bad = "No_Such_Locale_Zzz.99999";

TC_CASE(setlocale_bad_returns_null) { setlocale(LC_ALL, "C"); CHECK(setlocale(LC_ALL, (const char *)bad) == NULL); }
TC_CASE(setlocale_bad_leaves_locale_unchanged)
{
    const char *r;
    setlocale(LC_ALL, "C");
    setlocale(LC_ALL, (const char *)bad);
    r = setlocale(LC_CTYPE, NULL);
    CHECK(r != NULL);
    CHECK_STR(r, "C");
    CHECK_EQ_I(isalpha(0xE9), 0);
}
TC_CASE(create_locale_bad_returns_null)
{
    void *(__cdecl *cr)(int, const char *) = (void *(__cdecl *)(int, const char *))tc_crt_proc("_create_locale");
    void *l;
    if (!cr) TC_SKIP("_create_locale not exported");
    l = cr(LC_ALL, (const char *)bad);
    CHECK(l == NULL);
    if (l) { void (__cdecl *fr)(void *) = (void (__cdecl *)(void *))tc_crt_proc("_free_locale"); if (fr) fr(l); }
}
int main(void)
{
    tc_begin();
    TC_RUN(setlocale_bad_returns_null); TC_RUN(setlocale_bad_leaves_locale_unchanged);
    TC_RUN(create_locale_bad_returns_null);
    return tc_end();
}
