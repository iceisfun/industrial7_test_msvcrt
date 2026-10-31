/* TC
 * id:    stl_rand_lcg_sequence
 * area:  stdlib
 * funcs: srand rand
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   Microsoft CRT rand is the LCG state = state*214013 + 2531011, result (state >> 16) & 0x7fff (all CRT generations)
 * note:  srand(1) yields 41, 18467, 6334, 26500, 19169, 15724, 11478, 29358, 26962, 24464 (widely observed).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(seed1_known)
{
    static const int w[10] = {41, 18467, 6334, 26500, 19169, 15724, 11478, 29358, 26962, 24464};
    int i; srand(1);
    for (i = 0; i < 10; i++) CHECK_EQ_I(rand(), w[i]);
}
TC_CASE(matches_lcg)
{
    static const unsigned seeds[4] = {0u, 42u, 12345u, 0xFFFFFFFFu};
    int k, i;
    for (k = 0; k < 4; k++) {
        unsigned s = seeds[k]; srand(s);
        for (i = 0; i < 20; i++) { s = s * 214013u + 2531011u; CHECK_EQ_I(rand(), (int)((s >> 16) & 0x7fff)); }
    }
}
int main(void) { tc_begin(); TC_RUN(seed1_known); TC_RUN(matches_lcg); return tc_end(); }
