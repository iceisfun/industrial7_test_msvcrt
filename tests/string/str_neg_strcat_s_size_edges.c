/* TC
 * id:    str_neg_strcat_s_size_edges
 * area:  string
 * funcs: strcat_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strcat_s (ERANGE when dest too small; dest[0] set to 0; invalid parameter handler invoked)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

TC_CASE(one_short)
{
    char d[8] = "ab"; volatile const char *s = "cd"; int r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = strcat_s(d, 4, (const char *)s);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(d[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(empty_dest_one_char)
{
    char d[2] = ""; volatile const char *s = "a"; int r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = strcat_s(d, 1, (const char *)s);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(d[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(empty_both_control)
{
    char d[2] = ""; volatile const char *s = ""; int r;
    tc_iph_install(); tc_iph_count = 0;
    r = strcat_s(d, 1, (const char *)s);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(exact_fit_control)
{
    char d[8] = "ab"; volatile const char *s = "cd"; int r;
    tc_iph_install(); tc_iph_count = 0;
    r = strcat_s(d, 5, (const char *)s);
    CHECK_EQ_I(r, 0);
    CHECK_STR(d, "abcd");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(one_short); TC_RUN(empty_dest_one_char); TC_RUN(empty_both_control); TC_RUN(exact_fit_control); return tc_end(); }
