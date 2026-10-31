/* TC
 * id:    hp_aligned_offset_recalloc
 * area:  heap
 * funcs: _aligned_offset_recalloc _aligned_offset_malloc _aligned_msize _aligned_free
 * crt:   >=90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _aligned_offset_recalloc
 * note:  zero-tail check starts at _aligned_msize(old block): slack between requested and usable size is not
 *        "newly allocated" and need not be zero.
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_a = 64, v_off = 16;

TC_CASE(grow_zero_tail)
{
    unsigned char *p = (unsigned char *)_aligned_offset_malloc(30, v_a, v_off), *q;
    size_t i, m;
    CHECK(p != NULL); if (!p) return;
    m = _aligned_msize(p, v_a, v_off); if (m < 30) m = 30;
    for (i = 0; i < 30; i++) p[i] = (unsigned char)(0x40 + i);
    q = (unsigned char *)_aligned_offset_recalloc(p, 20, 20, v_a, v_off);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U(((uintptr_t)q + 16) % 64, 0);
    for (i = 0; i < 30; i++) if (q[i] != (unsigned char)(0x40 + i)) { CHECK_EQ_I(q[i], 0x40 + i); break; }
    for (i = m; i < 400; i++) if (q[i] != 0) { CHECK_EQ_I(q[i], 0); break; }
    _aligned_free(q);
}
int main(void) { tc_begin(); TC_RUN(grow_zero_tail); return tc_end(); }
