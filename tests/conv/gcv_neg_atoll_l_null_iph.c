/* TC
 * id:    gcv_neg_atoll_l_null_iph
 * area:  conv
 * funcs: _atoll_l _create_locale _free_locale
 * crt:   >=120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs atoi, _atoi_l (NULL str: invalid parameter handler, EINVAL, returns 0)
 * note:  tc_iph_install requires _set_invalid_parameter_handler (all targets here have it).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

TC_CASE(null_null_locale)
{
    volatile const char *s = NULL; long long r;
    tc_iph_count = 0; errno = 0;
    r = _atoll_l((const char *)s, NULL);
    CHECK_EQ_I(r, 0LL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_valid_locale)
{
    volatile const char *s = NULL; long long r; _locale_t L = _create_locale(LC_ALL, "C");
    CHECK(L != NULL);
    tc_iph_count = 0; errno = 0;
    r = _atoll_l((const char *)s, L);
    CHECK_EQ_I(r, 0LL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0;
    r = _atoll_l((const char *)s, L);
    CHECK_EQ_I(r, 0LL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 2);
    _free_locale(L);
}

int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_null_locale); TC_RUN(null_valid_locale);
    return tc_end();
}
