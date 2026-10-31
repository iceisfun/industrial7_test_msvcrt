/* TC
 * id:    mb_neg_mbsnbcmp_zero_count
 * area:  mbcs
 * funcs: _mbsnbcmp _mbsnbicmp _mbsncmp _mbsnicmp
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _mbsnbcmp: (inferred) compares at most count bytes/characters (zero compares nothing, so the strings are equal)
 * note:  count 0 on different valid strings returns 0 and does not invoke the handler.
 */
#include "tc.h"
#include <mbstring.h>
static const unsigned char a[] = "abc", b[] = "xyz";
static const unsigned char * volatile ap = a;
static const unsigned char * volatile bp = b;
TC_CASE(zero_count_equal)
{
    tc_iph_count = 0;
    CHECK_EQ_I(_mbsnbcmp(ap, bp, 0), 0);
    CHECK_EQ_I(_mbsnbicmp(ap, bp, 0), 0);
    CHECK_EQ_I(_mbsncmp(ap, bp, 0), 0);
    CHECK_EQ_I(_mbsnicmp(ap, bp, 0), 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(nonzero_count_differs)
{
    CHECK(_mbsnbcmp(ap, bp, 1) < 0);
    CHECK(_mbsnbcmp(bp, ap, 1) > 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(zero_count_equal); TC_RUN(nonzero_count_differs); return tc_end(); }
