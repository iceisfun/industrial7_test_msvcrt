/* TC
 * id:    gsm_neg_strlwr_strupr_s_invalid
 * area:  string
 * kind:  negative
 * funcs: _strlwr_s _strupr_s
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _strlwr_s, _strupr_s (EINVAL via invalid parameter handler for NULL str or str not NUL-terminated within numberOfElements)
 * note:  Buffer state after failure and the (NULL, 0) combination are probed in gsm_neg_strlwr_s_state_probe. msvcr80 omitted: mingw import lib lacks the _s symbols.
 */
#include "tc.h"
#define MINGW_HAS_SECURE_API 1
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
TC_CASE(unterminated)
{
    char b[16] = "ABCdef";
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(b, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    strcpy(b, "abcDEF");
    CHECK_EQ_I(_strupr_s(b, 3), EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(size_equals_len)
{
    char b[16] = "ABCD", c[16] = "abcd";
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(b, 4), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(_strupr_s(c, 4), EINVAL); CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(size_len_plus_one_ok)
{
    char b[16] = "ABCD", c[16] = "abcd";
    tc_iph_count = 0;
    CHECK_EQ_I(_strlwr_s(b, 5), 0); CHECK_STR(b, "abcd");
    CHECK_EQ_I(_strupr_s(c, 5), 0); CHECK_STR(c, "ABCD");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); if (!tc_iph_install()) return 3;
    TC_RUN(null_buf); TC_RUN(size_zero); TC_RUN(unterminated); TC_RUN(size_equals_len); TC_RUN(size_len_plus_one_ok);
    return tc_end();
}
