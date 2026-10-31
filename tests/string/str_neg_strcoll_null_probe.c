/* TC
 * id:    str_neg_strcoll_null_probe
 * area:  string
 * funcs: strcoll
 * crt:   >=100
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   none
 * note:  PROBE: strcoll(NULL, "a") and strcoll("a", NULL) return _NLSCMPERROR (INT_MAX), set errno=EINVAL and invoke the invalid-parameter handler once.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <limits.h>

static volatile const char *pa, *pb;

TC_CASE(first_null)
{
    int r; tc_iph_count = 0; errno = 0; pa = NULL; pb = "a";
    r = strcoll((const char *)pa, (const char *)pb);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(second_null)
{
    int r; tc_iph_count = 0; errno = 0; pa = "a"; pb = NULL;
    r = strcoll((const char *)pa, (const char *)pb);
    CHECK_EQ_I(r, INT_MAX); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(first_null); TC_RUN(second_null);
    return tc_end();
}
