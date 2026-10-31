/* TC
 * id:    str_neg_strnicoll_null_params
 * area:  string
 * funcs: _strnicoll
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcoll, wcscoll, _mbscoll, _strcoll_l (Return value: _NLSCMPERROR on error; Parameter validation)
 * note:  Companion to the _stricoll and strcoll null tests; one function per test so a crash in one does not mask the others.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

static volatile const char *pa, *pb;

TC_CASE(strnicoll_null)
{
    int r; tc_iph_count = 0; errno = 0; pa = NULL; pb = "a";
    r = _strnicoll((const char *)pa, (const char *)pb, 1);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; errno = 0; pa = "a"; pb = NULL;
    r = _strnicoll((const char *)pa, (const char *)pb, 1);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(strnicoll_null);
    return tc_end();
}
