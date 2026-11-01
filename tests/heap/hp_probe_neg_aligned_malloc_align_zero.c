/* TC
 * id:    hp_probe_neg_aligned_malloc_align_zero
 * area:  heap
 * funcs: _aligned_malloc
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _aligned_malloc (alignment must be power of 2)
 * note:  OBSERVED on Win10 22H2 ucrtbase: _aligned_malloc(16, 0) returns NULL, errno EINVAL, one invalid-parameter call
 *        (alignment 0 is not a power of two for the UCRT). The old "accepts 0" guess was wrong.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

TC_CASE(align_zero)
{
    volatile size_t a = 0, sz = 16; void *p;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    p = _aligned_malloc(sz, a);
    CHECK(p == NULL);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (p) _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(align_zero);
    return tc_end();
}
