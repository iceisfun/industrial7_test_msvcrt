/* TC
 * id:    str_neg_strpbrk_strspn_empty_sets
 * area:  string
 * funcs: strpbrk strspn strcspn
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.5.4, 7.24.5.3, 7.24.5.6; MS docs strpbrk, strspn, strcspn
 */
#include "tc.h"
#include <string.h>

static const char *volatile abc = "abc", *volatile none = "", *volatile xyz = "xyz";

TC_CASE(pbrk)
{
    CHECK_EQ_P(strpbrk(abc, none), NULL);
    CHECK_EQ_P(strpbrk(none, abc), NULL);
    CHECK_EQ_P(strpbrk(abc, xyz), NULL);
}
TC_CASE(spn)
{
    CHECK_EQ_U(strspn(abc, none), 0);
    CHECK_EQ_U(strspn(none, abc), 0);
    CHECK_EQ_U(strspn(abc, xyz), 0);
}
TC_CASE(cspn)
{
    CHECK_EQ_U(strcspn(abc, none), 3);
    CHECK_EQ_U(strcspn(none, abc), 0);
    CHECK_EQ_U(strcspn(abc, abc), 0);
    CHECK_EQ_U(strcspn(abc, xyz), 3);
}
int main(void) { tc_begin(); TC_RUN(pbrk); TC_RUN(spn); TC_RUN(cspn); return tc_end(); }
