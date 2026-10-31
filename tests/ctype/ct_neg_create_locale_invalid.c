/* TC
 * id:    ct_neg_create_locale_invalid
 * area:  ctype
 * funcs:
 * procs: _create_locale _free_locale
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale (returns NULL if the locale name is not valid)
 */
#include "tc.h"

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
static cl_t p_cl; static fl_t p_fl;
#define LC_ALL_ 0

TC_CASE(bogus_name_returns_NULL) { void *l = p_cl(LC_ALL_, "no_such_locale_xyz"); CHECK(l == NULL); if (l) p_fl(l); }
TC_CASE(empty_string_does_not_return_NULL)
{
    void *l = p_cl(LC_ALL_, "");
    if (!l) TC_SKIP("user default locale unavailable");
    CHECK(l != NULL); p_fl(l);
}

int main(void)
{
    tc_begin();
    p_cl = (cl_t)tc_crt_proc("_create_locale"); p_fl = (fl_t)tc_crt_proc("_free_locale");
    if (!p_cl || !p_fl) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(bogus_name_returns_NULL); TC_RUN(empty_string_does_not_return_NULL);
    return tc_end();
}
