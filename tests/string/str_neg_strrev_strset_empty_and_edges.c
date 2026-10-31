/* TC
 * id:    str_neg_strrev_strset_empty_and_edges
 * area:  string
 * funcs: _strrev _strset _strnset _strlwr _strupr
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _strrev, _strset, _strnset, _strlwr, _strupr (operate up to the terminating NUL)
 */
#include "tc.h"
#include <string.h>

struct buf { char b[4]; char g[4]; };
#define MK(n, s) struct buf n = { s, { 'G', 'G', 'G', 'G' } }
#define GUARD(n) CHECK_MEM((n).g, "GGGG", 4)

TC_CASE(strrev_empty_and_one)
{
    MK(e, ""); MK(o, "a");
    CHECK_EQ_P(_strrev(e.b), e.b); CHECK_EQ_I(e.b[0], 0); GUARD(e);
    CHECK_EQ_P(_strrev(o.b), o.b); CHECK_STR(o.b, "a"); GUARD(o);
}
TC_CASE(strset_empty)
{
    MK(e, ""); volatile int c = 'x';
    _strset(e.b, c);
    CHECK_EQ_I(e.b[0], 0); GUARD(e);
}
TC_CASE(strnset_edges)
{
    MK(a, "abc"); MK(b, "ab"); volatile int c = 'x'; volatile size_t n = 0;
    _strnset(a.b, c, n);
    CHECK_STR(a.b, "abc"); GUARD(a);
    n = 100;
    _strnset(b.b, c, n);
    CHECK_STR(b.b, "xx"); GUARD(b);
}
TC_CASE(case_empty)
{
    MK(l, ""); MK(u, "");
    _strlwr(l.b); _strupr(u.b);
    CHECK_EQ_I(l.b[0], 0); CHECK_EQ_I(u.b[0], 0);
    GUARD(l); GUARD(u);
}
int main(void) { tc_begin(); TC_RUN(strrev_empty_and_one); TC_RUN(strset_empty); TC_RUN(strnset_edges); TC_RUN(case_empty); return tc_end(); }
