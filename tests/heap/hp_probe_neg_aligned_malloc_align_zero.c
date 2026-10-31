/* TC
 * id:    hp_probe_neg_aligned_malloc_align_zero
 * area:  heap
 * funcs: _aligned_malloc
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_malloc (alignment must be power of 2)
 * note:  PROBING alignment 0. The CRT's power-of-2 test is believed to be (a & (a-1)) == 0, which accepts 0, after which alignment is raised
 *        to at least sizeof(void*); so expected = success, pointer-aligned, no IPH, errno untouched. Docs only say 'power of 2'; 0 may instead be rejected.
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
    CHECK(p != NULL);
    CHECK_EQ_I(errno, 0);
    CHECK_EQ_I(tc_iph_count, 0);
    if (p) CHECK_EQ_U((uintptr_t)p % sizeof(void *), 0);
    if (p) _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(align_zero);
    return tc_end();
}
