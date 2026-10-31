/* TC
 * id:    hp_neg_aligned_malloc_null_result_no_iph_on_nomem
 * area:  heap
 * funcs: _aligned_malloc _aligned_offset_malloc malloc calloc realloc _expand free
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _aligned_malloc / malloc / calloc / realloc / _expand: NULL when memory is insufficient; invalid-parameter handler is only for invalid arguments (aligned: _aligned_malloc page)
 * note:  errno ENOMEM is asserted for malloc/calloc/realloc (documented); not for _expand nor the aligned functions
 *        (aligned errno is probed in hp_neg_aligned_nomem_errno_probe).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_huge = SIZE_MAX / 2, v_16 = 16, v_8 = 8, v_1 = 1;

TC_CASE(aligned_malloc)
{
    void *p;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    p = _aligned_malloc(v_huge, v_16);
    CHECK(p == NULL); CHECK_EQ_I(tc_iph_count, 0);
    if (p) _aligned_free(p);
}
TC_CASE(aligned_offset_malloc)
{
    void *p;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    p = _aligned_offset_malloc(v_huge, v_16, v_8);
    CHECK(p == NULL); CHECK_EQ_I(tc_iph_count, 0);
    if (p) _aligned_free(p);
}
TC_CASE(plain_family)
{
    void *p, *q, *s;
    tc_iph_install(); tc_iph_count = 0;
    errno = 0; p = malloc(v_huge); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); if (p) free(p);
    errno = 0; p = calloc(v_1, v_huge); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); if (p) free(p);
    s = malloc(v_16); CHECK(s != NULL);
    if (s) {
        errno = 0; q = realloc(s, v_huge); CHECK(q == NULL); CHECK_EQ_I(errno, ENOMEM);
        if (q) s = q;
        q = _expand(s, v_huge); CHECK(q == NULL);
        free(s);
    }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(aligned_malloc); TC_RUN(aligned_offset_malloc); TC_RUN(plain_family);
    return tc_end();
}
