/* TC
 * id:    loc_neg_wsetlocale_bad_category
 * area:  locale
 * funcs: _wsetlocale setlocale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale, _wsetlocale (returns NULL if category or locale is invalid)
 * note:  errno/invalid-parameter checks are limited to msvcr80+ (msvcr80 and later validate the category via the parameter-validation macros); the NULL return is checked everywhere.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>

static volatile int v_cat;
static const wchar_t *volatile w_c = L"C";

TC_CASE(minus_one)
{
    v_cat = -1; errno = 0; tc_iph_count = 0;
    CHECK(_wsetlocale(v_cat, w_c) == NULL);
    if (TC_AT_LEAST(80)) { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
}
TC_CASE(max_plus_one)
{
    v_cat = LC_MAX + 1; errno = 0; tc_iph_count = 0;
    CHECK(_wsetlocale(v_cat, w_c) == NULL);
    if (TC_AT_LEAST(80)) { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
}
TC_CASE(hundred)
{
    v_cat = 100; errno = 0; tc_iph_count = 0;
    CHECK(_wsetlocale(v_cat, w_c) == NULL);
    if (TC_AT_LEAST(80)) { CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); }
}
TC_CASE(state_unchanged) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(minus_one); TC_RUN(max_plus_one); TC_RUN(hundred); TC_RUN(state_unchanged);
    return tc_end();
}
