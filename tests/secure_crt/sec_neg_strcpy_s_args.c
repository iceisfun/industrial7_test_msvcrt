/* TC
 * id:    sec_neg_strcpy_s_args
 * area:  secure_crt
 * kind:  negative
 * funcs: strcpy_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcpy_s error conditions table
 * note:  size 0: the docs table says ERANGE, the implementation is believed to return EINVAL; this test accepts either
 *        (exact value probed in sec_neg_strcpy_s_size0_probe).
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

static volatile size_t v_zero;
TC_CASE(null_dest_EINVAL) { tc_iph_count = 0; CHECK_EQ_I(strcpy_s(NULL, 4, "a"), EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(size_zero_fails) { char d[4] = "xyz"; int r; tc_iph_count = 0; r = strcpy_s(d, v_zero, "a"); CHECK(r == EINVAL || r == ERANGE); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(null_src_EINVAL_dest_emptied) { char d[4] = "xyz"; tc_iph_count = 0; CHECK_EQ_I(strcpy_s(d, sizeof d, NULL), EINVAL); CHECK_EQ_I(d[0], 0); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(handler_count_one_per_call)
{ char d[4]; tc_iph_count = 0; strcpy_s(NULL, 4, "a"); strcpy_s(d, 4, NULL); strcpy_s(d, 2, "abc"); CHECK_EQ_I(tc_iph_count, 3); }
TC_CASE(errno_EINVAL) { errno = 0; strcpy_s(NULL, 4, "a"); CHECK_EQ_I(errno, EINVAL); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(null_dest_EINVAL); TC_RUN(size_zero_fails); TC_RUN(null_src_EINVAL_dest_emptied); TC_RUN(handler_count_one_per_call); TC_RUN(errno_EINVAL);
    return tc_end();
}
