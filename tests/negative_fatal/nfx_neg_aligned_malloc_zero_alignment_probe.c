/* TC
 * id:    nfx_neg_aligned_malloc_zero_alignment_probe
 * area:  negative_fatal
 * funcs: _aligned_malloc _aligned_free _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_malloc
 * note:  PROBE: alignment 0. The power-of-two check is (a & (a-1)) == 0, which 0 satisfies, and the alignment is then
 *        raised to the pointer size, so we expect a non-NULL pointer and NO invalid-parameter call.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

static volatile size_t sz = 16, al0 = 0;
TC_CASE(zero)
{
    void *p;
    tc_iph_count = 0;
    p = _aligned_malloc(sz, al0);
    CHECK(p != NULL);
    CHECK_EQ_I(tc_iph_count, 0);
    _aligned_free(p);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(zero); return tc_end(); }
