/* TC
 * id:    loc_neg_strlwr_l_null
 * area:  locale
 * funcs: _strlwr_l _strupr_l _wcslwr_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _strlwr_l
 * note:  PROBE: NULL string returns NULL, errno EINVAL, handler invoked (validation macro in the non-_s variants).
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <errno.h>

static char *volatile v_null_s;
static wchar_t *volatile v_null_w;
static _locale_t loc;

TC_CASE(strlwr_l_null)
{
    int have = tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK(_strlwr_l(v_null_s, loc) == NULL); CHECK_EQ_I(errno, EINVAL);
    if (have) CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(strupr_l_null)
{
    int have = tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK(_strupr_l(v_null_s, loc) == NULL); CHECK_EQ_I(errno, EINVAL);
    if (have) CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(wcslwr_l_null)
{
    int have = tc_iph_install(); tc_iph_count = 0; errno = 0;
    CHECK(_wcslwr_l(v_null_w, loc) == NULL); CHECK_EQ_I(errno, EINVAL);
    if (have) CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(strlwr_l_null); TC_RUN(strupr_l_null); TC_RUN(wcslwr_l_null);
    _free_locale(loc);
    return tc_end();
}
