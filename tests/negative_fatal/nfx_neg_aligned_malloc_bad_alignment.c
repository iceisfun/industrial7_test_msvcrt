/* TC
 * id:    nfx_neg_aligned_malloc_bad_alignment
 * area:  negative_fatal
 * funcs: _aligned_malloc _aligned_free _aligned_realloc _aligned_offset_malloc _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_malloc (alignment must be a power of 2), _aligned_offset_malloc (offset must be < size), _aligned_free
 * note:  Invalid arguments return NULL, set errno to EINVAL and call the invalid-parameter handler. Alignment 0 is in a separate probe test.
 */
#include "tc.h"
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>

static volatile size_t al3 = 3, sz = 16, al16 = 16, off32 = 32;

TC_CASE(non_pow2)
{
    tc_iph_count = 0; errno = 0;
    CHECK(_aligned_malloc(sz, al3) == NULL);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(offset_ge_size)
{
    tc_iph_count = 0; errno = 0;
    CHECK(_aligned_offset_malloc(sz, al16, off32) == NULL);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(realloc_bad)
{
    void *p = _aligned_malloc(sz, al16);
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    CHECK(_aligned_realloc(p, 32, al3) == NULL);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    ((char *)p)[15] = 1;           /* original block still valid */
    _aligned_free(p);
}
TC_CASE(free_null)
{
    tc_iph_count = 0;
    _aligned_free(NULL);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(non_pow2); TC_RUN(offset_ge_size); TC_RUN(realloc_bad); TC_RUN(free_null);
    return tc_end();
}
