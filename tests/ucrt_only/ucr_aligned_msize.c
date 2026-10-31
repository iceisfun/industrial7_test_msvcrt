/* TC
 * id:    ucr_aligned_msize
 * area:  ucrt_only
 * funcs: _aligned_msize _aligned_malloc _aligned_free _aligned_offset_malloc
 * crt:   win10 msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _aligned_msize (returns the size of a block allocated by _aligned_malloc/_aligned_offset_malloc)
 * note:  asserts only usable size >= requested; the exact-size expectation lives in ucr_aligned_msize_exact_probe.
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_size, v_align, v_off;

TC_CASE(at_least_requested)
{
    static const size_t sizes[] = { 1, 16, 100, 4096 };
    static const size_t aligns[] = { 16, 64 };
    int i, j;
    for (i = 0; i < 4; i++) for (j = 0; j < 2; j++) {
        void *p; v_size = sizes[i]; v_align = aligns[j];
        p = _aligned_malloc(v_size, v_align);
        CHECK(p != NULL);
        if (!p) continue;
        CHECK_EQ_U((size_t)(ULONG_PTR)p % aligns[j], 0);
        CHECK(_aligned_msize(p, v_align, 0) >= sizes[i]);
        _aligned_free(p);
    }
}
TC_CASE(offset_variant)
{
    void *p; v_size = 200; v_align = 32; v_off = 8;
    p = _aligned_offset_malloc(v_size, v_align, v_off);
    CHECK(p != NULL);
    if (!p) return;
    CHECK_EQ_U(((size_t)(ULONG_PTR)p + 8) % 32, 0);
    CHECK(_aligned_msize(p, v_align, v_off) >= 200);
    _aligned_free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(at_least_requested); TC_RUN(offset_variant);
    return tc_end();
}
