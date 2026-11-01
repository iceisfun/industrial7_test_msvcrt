/* TC
 * id:    nfx_neg_aligned_malloc_zero_alignment_probe
 * area:  negative_fatal
 * funcs: _aligned_malloc _aligned_free _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _aligned_malloc
 * note:  OBSERVED on Win10 22H2 (19045): _aligned_malloc(16, 0) returns NULL, errno EINVAL and calls the invalid-parameter
 *        handler once (zero is not accepted as a power of two). The earlier expectation (non-NULL, no handler call) was wrong.
 *        Stable.
 */
#include "tc.h"
#include <malloc.h>
#include <errno.h>

static volatile size_t sz = 16, al0 = 0;
TC_CASE(zero)
{
    void *p;
    tc_iph_count = 0;
    errno = 0;
    p = _aligned_malloc(sz, al0);
    CHECK(p == NULL);                 /* observed: alignment 0 is rejected */
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    _aligned_free(p);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(zero); return tc_end(); }
