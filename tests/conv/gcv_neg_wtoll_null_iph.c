/* TC
 * id:    gcv_neg_wtoll_null_iph
 * area:  conv
 * funcs: _wtoll _wtoll_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _wtoi, _wtoi_l
 * note:  NULL string with handler installed.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

TC_CASE(wtoll_null)
{
    const wchar_t *volatile s = NULL; long long r;
    tc_iph_count = 0; errno = 0;
    r = _wtoll(s);
    CHECK_EQ_I(r, 0LL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wtoll_l_null)
{
    const wchar_t *volatile s = NULL; long long r; _locale_t L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    tc_iph_count = 0; errno = 0;
    r = _wtoll_l(s, L);
    CHECK_EQ_I(r, 0LL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    _free_locale(L);
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(wtoll_null); TC_RUN(wtoll_l_null);
    return tc_end();
}
