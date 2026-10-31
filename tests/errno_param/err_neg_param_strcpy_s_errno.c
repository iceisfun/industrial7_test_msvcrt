/* TC
 * id:    err_neg_param_strcpy_s_errno
 * area:  errno_param
 * kind:  negative
 * funcs: strcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcpy_s (error conditions table)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static char * volatile v_null = NULL;
static volatile size_t v_sz;

TC_CASE(dest_null)
{
    int r; errno = 0; tc_iph_count = 0;
    r = strcpy_s(v_null, 10, "abc");
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    char d[4] = "q"; int r; errno = 0; tc_iph_count = 0; v_sz = 0;
    r = strcpy_s(d, v_sz, "abc");
    CHECK(r != 0); CHECK(errno == EINVAL || errno == ERANGE); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(src_null)
{
    char d[4] = "qqq"; const char * volatile s = NULL; int r; errno = 0; tc_iph_count = 0;
    r = strcpy_s(d, sizeof d, s);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(too_small)
{
    char d[8] = "qqqqqqq"; int r; errno = 0; tc_iph_count = 0; v_sz = 2;
    r = strcpy_s(d, v_sz, "abcde");
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(total_count)
{
    char d[2]; const char * volatile s = NULL; int base;
    tc_iph_count = 0;
    strcpy_s(v_null, 10, "a"); v_sz = 0; strcpy_s(d, v_sz, "a"); strcpy_s(d, sizeof d, s); v_sz = 1; strcpy_s(d, v_sz, "ab");
    base = tc_iph_count;
    CHECK_EQ_I(base, 4);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(dest_null); TC_RUN(size_zero); TC_RUN(src_null); TC_RUN(too_small); TC_RUN(total_count);
    return tc_end();
}
