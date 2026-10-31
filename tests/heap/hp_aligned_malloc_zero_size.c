/* TC
 * id:    hp_aligned_malloc_zero_size
 * area:  heap
 * funcs: _aligned_malloc _aligned_free
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _aligned_malloc
 * note:  PROBING: _aligned_malloc(0, 16) returns a valid non-NULL pointer that is 16-byte aligned (the aligned
 *        allocator adds header space, so a zero-size request is expected to succeed). The docs do not state this.
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_size = 0, v_align = 16;

TC_CASE(zero_size)
{
    void *p = _aligned_malloc(v_size, v_align);
    CHECK(p != NULL);
    if (p) { CHECK_EQ_U((uintptr_t)p % 16, 0); _aligned_free(p); }
}
int main(void) { tc_begin(); TC_RUN(zero_size); return tc_end(); }
