/* TC
 * id:    stl_neg_swab_negative_n_probe
 * area:  stdlib
 * kind:  negative
 * funcs: _swab
 * crt:   >=100
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _swab ("n should be even")
 * note:  Probes (a) n<0 writes nothing; OBSERVED on Win10 22H2 ucrtbase it DOES raise one invalid-parameter call (old
 *        expectation: none), (b) n==1 writes nothing, (c) odd n=3 swaps only the first pair and leaves the third dest byte
 *        untouched. Each probe is in its own case.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

TC_CASE(negative_n_no_write)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    _swab((char *)"abcd", d, -2);
    CHECK_MEM(d, "xxxx", 4);
}
TC_CASE(negative_n_iph)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    tc_iph_count = 0;
    _swab((char *)"abcd", d, -2);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(n1_untouched)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    _swab((char *)"abcd", d, 1);
    CHECK_MEM(d, "xxxx", 4);
}
TC_CASE(odd_n3)
{
    char d[4] = { 'x', 'x', 'x', 'x' };
    _swab((char *)"abcd", d, 3);
    CHECK_MEM(d, "baxx", 4);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(negative_n_no_write); TC_RUN(negative_n_iph); TC_RUN(n1_untouched); TC_RUN(odd_n3);
    return tc_end();
}
