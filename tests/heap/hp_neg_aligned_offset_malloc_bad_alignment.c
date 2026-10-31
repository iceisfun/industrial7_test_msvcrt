/* TC
 * id:    hp_neg_aligned_offset_malloc_bad_alignment
 * area:  heap
 * funcs: _aligned_offset_malloc
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_offset_malloc: alignment must be a power of 2
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(bad)
{
    static const size_t al[] = { 3, 48, 1000 };
    int i;
    CHECK(tc_iph_install());
    for (i = 0; i < 3; i++) {
        volatile size_t a = al[i], sz = 64, off = 4; void *p;
        tc_iph_count = i; errno = 0;
        p = _aligned_offset_malloc(sz, a, off);
        CHECK(p == NULL);
        CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(tc_iph_count, i + 1);
        if (p) _aligned_free(p);
    }
}
int main(void)
{
    tc_begin();
    TC_RUN(bad);
    return tc_end();
}
