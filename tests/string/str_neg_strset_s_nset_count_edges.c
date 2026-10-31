/* TC
 * id:    str_neg_strset_s_nset_count_edges
 * area:  string
 * funcs: _strnset_s _strset_s
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strset_s, _strnset_s, _wcsset_s, _mbsset_s
 */
#include "tc.h"
#include <string.h>
#include <errno.h>

struct buf { char b[8]; char g[8]; };

TC_CASE(count_larger_than_string)
{
    struct buf s = { "ab", "GGGGGGG" }; volatile int c = 'z'; volatile size_t cnt = 100;
    CHECK_EQ_I(_strnset_s(s.b, 8, c, cnt), 0);
    CHECK_STR(s.b, "zz");
    CHECK_STR(s.g, "GGGGGGG");
}
TC_CASE(count_zero)
{
    struct buf s = { "abc", "GGGGGGG" }; volatile int c = 'z'; volatile size_t cnt = 0;
    CHECK_EQ_I(_strnset_s(s.b, 8, c, cnt), 0);
    CHECK_STR(s.b, "abc");
}
TC_CASE(unterminated)
{
    struct buf s; volatile int c = 'z'; volatile size_t cnt = 2; int r;
    memset(&s, 'a', 8); memset(s.g, 'G', 8);
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _strnset_s(s.b, 4, c, cnt);
    CHECK_EQ_I(r, EINVAL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(s.b[4], 'a');
    CHECK_EQ_I(s.g[0], 'G');
}
TC_CASE(strset_empty)
{
    struct buf s = { "", "GGGGGGG" }; volatile int c = 'x';
    CHECK_EQ_I(_strset_s(s.b, 8, c), 0);
    CHECK_EQ_I(s.b[0], 0);
    CHECK_STR(s.g, "GGGGGGG");
}
int main(void) { tc_begin(); TC_RUN(count_larger_than_string); TC_RUN(count_zero); TC_RUN(unterminated); TC_RUN(strset_empty); return tc_end(); }
