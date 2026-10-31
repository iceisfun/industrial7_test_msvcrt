/* TC
 * id:    hp_aligned_msize
 * area:  heap
 * funcs: _aligned_msize _aligned_malloc _aligned_free
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _aligned_msize
 * note:  msvcr80 and msvcrt.dll do not export _aligned_msize (msvcr80 exports are not relied on here).
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_sizes[3] = { 1, 100, 4096 }, v_a = 16, v_off = 0, v_a64 = 64;

TC_CASE(sizes_align16)
{
    int i;
    for (i = 0; i < 3; i++) {
        size_t n = v_sizes[i]; void *p = _aligned_malloc(n, v_a);
        CHECK(p != NULL); if (!p) continue;
        CHECK(_aligned_msize(p, v_a, v_off) >= n);
        _aligned_free(p);
    }
}
TC_CASE(align64)
{
    void *p = _aligned_malloc(300, v_a64);
    CHECK(p != NULL); if (!p) return;
    CHECK(_aligned_msize(p, v_a64, v_off) >= 300);
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(sizes_align16); TC_RUN(align64); return tc_end(); }
