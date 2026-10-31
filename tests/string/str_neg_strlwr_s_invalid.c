/* TC
 * id:    str_neg_strlwr_s_invalid
 * area:  string
 * funcs: _strlwr_s _strupr_s
 * procs: 
 * crt:   >=100
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strlwr_s (EINVAL for NULL buffer or string not null-terminated within size)
 * note:  Buffer contents after failure are not asserted.
 */
#include "tc.h"
#include <string.h>
#include <errno.h>
TC_CASE(null_buf)
{
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(NULL, 8), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_strupr_s(NULL, 8), EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(size_zero)
{
    char b[8] = "ABC";
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(b, 0), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_strupr_s(b, 0), EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(unterminated_in_size)
{
    char b[16] = "ABCdef"; /* NUL lies beyond the declared size 3 */
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(b, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    strcpy(b, "abcDEF");
    CHECK_EQ_I(_strupr_s(b, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(size_equals_len)
{
    char b[16] = "ABC";
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(b, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); if (!tc_iph_install()) return 3; TC_RUN(null_buf); TC_RUN(size_zero); TC_RUN(unterminated_in_size); TC_RUN(size_equals_len); return tc_end(); }
