/* TC
 * id:    hp_neg_b2_aligned_nonpow2
 * area:  heap
 * funcs: _aligned_malloc _aligned_free
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_malloc (alignment must be an integer power of 2; invalid-parameter handler, EINVAL, NULL)
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_n, v_a;

TC_CASE(non_power_of_two)
{
    static const size_t bad[] = { 3, 12, 24, 100 };
    size_t i;
    CHECK(tc_iph_install());
    for (i = 0; i < sizeof bad / sizeof bad[0]; i++) {
        void *p; v_n = 64; v_a = bad[i]; errno = 0; tc_iph_count = 0;
        p = _aligned_malloc(v_n, v_a);
        CHECK(p == NULL);
        if (p) { _aligned_free(p); continue; }
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, 1);
    }
}
int main(void) { tc_begin(); TC_RUN(non_power_of_two); return tc_end(); }
