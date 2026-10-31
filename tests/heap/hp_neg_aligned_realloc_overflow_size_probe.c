/* TC
 * id:    hp_neg_aligned_realloc_overflow_size_probe
 * area:  heap
 * funcs: _aligned_realloc _aligned_offset_realloc _aligned_malloc _aligned_free
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _aligned_realloc (NULL on failure)
 * note:  PROBE: sizes near SIZE_MAX (SIZE_MAX-8, SIZE_MAX/2) with valid alignment: expected NULL (size + alignment + header
 *        must not wrap), errno ENOMEM, no invalid-parameter call. errno and iph for the aligned functions are not documented
 *        per page, hence probe. A wrapped size would return a tiny block, which these asserts would catch.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_big = SIZE_MAX - 8, v_half = SIZE_MAX / 2, v_16 = 16, v_64 = 64, v_0 = 0;

TC_CASE(realloc_half_errno)
{
    void *p, *r;
    tc_iph_install();
    p = _aligned_malloc(v_64, v_64);
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0; errno = 0;
    r = _aligned_realloc(p, v_half, v_64);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOMEM); CHECK_EQ_I(tc_iph_count, 0);
    _aligned_free(r ? r : p);
}
TC_CASE(realloc_wrap_size)
{
    void *p, *r;
    tc_iph_install();
    p = _aligned_malloc(v_64, v_64);
    CHECK(p != NULL); if (!p) return;
    tc_iph_count = 0;
    r = _aligned_realloc(p, v_big, v_64);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 0);
    _aligned_free(r ? r : p);
}
TC_CASE(offset_realloc_null_wrap_size)
{
    void *r;
    tc_iph_install(); tc_iph_count = 0;
    r = _aligned_offset_realloc(NULL, v_big, v_16, v_0);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 0);
    if (r) _aligned_free(r);
}
int main(void)
{
    tc_begin();
    TC_RUN(realloc_half_errno); TC_RUN(realloc_wrap_size); TC_RUN(offset_realloc_null_wrap_size);
    return tc_end();
}
