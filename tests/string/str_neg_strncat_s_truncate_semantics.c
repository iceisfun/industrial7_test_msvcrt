/* TC
 * id:    str_neg_strncat_s_truncate_semantics
 * area:  string
 * funcs: strncat_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strncat_s (_TRUNCATE returns STRUNCATE without invoking the handler; ERANGE otherwise)
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#ifndef STRUNCATE
#define STRUNCATE 80
#endif
static volatile const char *v_src = "defghij";

TC_CASE(truncate)
{
    char d[16] = "abc"; int r;
    tc_iph_install(); tc_iph_count = 0;
    r = strncat_s(d, 8, (const char *)v_src, _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE);
    CHECK_STR(d, "abcdefg");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_fits_control)
{
    char d[16] = "abc"; int r;
    tc_iph_install(); tc_iph_count = 0;
    r = strncat_s(d, 8, (const char *)v_src, 4);
    CHECK_EQ_I(r, 0);
    CHECK_STR(d, "abcdefg");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_too_big)
{
    char d[16] = "abc"; int r;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = strncat_s(d, 8, (const char *)v_src, 5);
    CHECK_EQ_I(r, ERANGE);
    CHECK_EQ_I(errno, ERANGE);
    CHECK_EQ_I(d[0], 0);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(full_dest_truncate)
{
    char d[16] = "abcdefg"; volatile const char *x = "x"; int r;
    tc_iph_install(); tc_iph_count = 0;
    r = strncat_s(d, 8, (const char *)x, _TRUNCATE);
    CHECK_EQ_I(r, STRUNCATE);
    CHECK_STR(d, "abcdefg");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(truncate); TC_RUN(count_fits_control); TC_RUN(count_too_big); TC_RUN(full_dest_truncate); return tc_end(); }
