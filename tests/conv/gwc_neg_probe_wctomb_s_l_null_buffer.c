/* TC
 * id:    gwc_neg_probe_wctomb_s_l_null_buffer
 * area:  conv
 * funcs: _wctomb_s_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wctomb_s, _wctomb_s_l (NULL buffer handling)
 * note:  PROBES the two NULL-buffer combinations separately. A NULL buffer with sizeInBytes > 0 is either a state query
 *        (returns 0, *pRet == 0 for the stateless C locale) or an invalid argument (EINVAL + handler). NULL with size 0 is
 *        either success or an error. The assertions only demand internal consistency (return/errno/handler/*pRet agree);
 *        the actual values are what the real-Windows run is meant to reveal.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <locale.h>
#include <wchar.h>

static _locale_t L;
static char *volatile vbuf;
static volatile size_t vsz;

TC_CASE(null_positive_size)
{
    int n = 99, r; tc_iph_count = 0; errno = 0; vbuf = NULL; vsz = 4;
    r = _wctomb_s_l(&n, vbuf, vsz, L'A', L);
    CHECK(r == 0 || r == EINVAL);
    if (r == 0) CHECK_EQ_I(n, 0);
    else { CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
}
TC_CASE(null_zero_size)
{
    int n = 99, r; tc_iph_count = 0; errno = 0; vbuf = NULL; vsz = 0;
    r = _wctomb_s_l(&n, vbuf, vsz, L'A', L);
    CHECK(r == 0 || r == EINVAL || r == ERANGE);
    if (r == 0) CHECK_EQ_I(tc_iph_count, 0);
    else CHECK_EQ_I(errno, r);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no invalid parameter handler API\n"); return tc_end(); }
    L = _create_locale(LC_ALL, "C");
    if (!L) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(null_positive_size); TC_RUN(null_zero_size);
    _free_locale(L);
    return tc_end();
}
