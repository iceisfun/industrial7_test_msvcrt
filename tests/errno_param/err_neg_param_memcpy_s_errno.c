/* TC
 * id:    err_neg_param_memcpy_s_errno
 * area:  errno_param
 * kind:  negative
 * funcs: memcpy_s _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs memcpy_s, wmemcpy_s (error conditions table)
 */
#include "tc.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

static void * volatile v_null = NULL;
static volatile size_t v_n;

TC_CASE(dest_null)
{
    char s[4] = "abc"; int r; errno = 0; tc_iph_count = 0; v_n = 3;
    r = memcpy_s(v_null, 8, s, v_n);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(src_null)
{
    char d[4]; const void * volatile s = NULL; int r, i; errno = 0; tc_iph_count = 0; v_n = 3;
    for (i = 0; i < 4; i++) d[i] = 'x';
    r = memcpy_s(d, sizeof d, s, v_n);
    CHECK_EQ_I(r, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 4; i++) CHECK_EQ_I(d[i], 0);
}
TC_CASE(count_too_big)
{
    char d[4], s[8] = "abcdefg"; int r, i; errno = 0; tc_iph_count = 0; v_n = 6;
    for (i = 0; i < 4; i++) d[i] = 'x';
    r = memcpy_s(d, sizeof d, s, v_n);
    CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1);
    for (i = 0; i < 4; i++) CHECK_EQ_I(d[i], 0);
}
int main(void)
{
    tc_begin();
    _set_invalid_parameter_handler(tc_iph);
    TC_RUN(dest_null); TC_RUN(src_null); TC_RUN(count_too_big);
    return tc_end();
}
