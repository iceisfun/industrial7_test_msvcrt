/* TC
 * id:    ucr_aligned_msize_exact_probe
 * area:  ucrt_only
 * funcs: _aligned_msize _aligned_malloc _aligned_free _aligned_offset_malloc
 * crt:   win10 msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _aligned_msize
 * note:  PROBE: whether _aligned_msize returns exactly the requested size (HeapSize of the underlying block minus the bookkeeping header). Docs only promise "size of the block".
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_size, v_align, v_off;

TC_CASE(exact_size)
{
    static const size_t sizes[] = { 1, 100, 4096 };
    int i;
    for (i = 0; i < 3; i++) {
        void *p; v_size = sizes[i]; v_align = 16;
        p = _aligned_malloc(v_size, v_align);
        CHECK(p != NULL);
        if (!p) continue;
        CHECK_EQ_U(_aligned_msize(p, v_align, 0), sizes[i]);
        _aligned_free(p);
    }
}
TC_CASE(exact_size_offset)
{
    void *p; v_size = 200; v_align = 32; v_off = 8;
    p = _aligned_offset_malloc(v_size, v_align, v_off);
    CHECK(p != NULL);
    if (!p) return;
    CHECK_EQ_U(_aligned_msize(p, v_align, v_off), 200);
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(exact_size); TC_RUN(exact_size_offset); return tc_end(); }
