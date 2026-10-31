/* TC
 * id:    hp_aligned_recalloc
 * area:  heap
 * funcs: _aligned_recalloc _aligned_msize _aligned_free
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _aligned_recalloc (count*size bytes, newly allocated bytes zero-initialised)
 * note:  the grow check starts at _aligned_msize(old block): bytes between the requested size and the usable size of
 *        the old block are not part of the "newly allocated" region and need not be zero.
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_a = 32, v_off = 0;

TC_CASE(null_acts_as_calloc)
{
    unsigned char *p = (unsigned char *)_aligned_recalloc(NULL, 4, 16, v_a);
    size_t i;
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U((uintptr_t)p % 32, 0);
    for (i = 0; i < 64; i++) if (p[i] != 0) { CHECK_EQ_I(p[i], 0); break; }
    _aligned_free(p);
}
TC_CASE(grow_zero_tail)
{
    unsigned char *p = (unsigned char *)_aligned_recalloc(NULL, 1, 40, v_a), *q;
    size_t i, m;
    CHECK(p != NULL); if (!p) return;
    m = _aligned_msize(p, v_a, v_off); if (m < 40) m = 40;
    for (i = 0; i < 40; i++) p[i] = (unsigned char)(i + 1);
    q = (unsigned char *)_aligned_recalloc(p, 10, 40, v_a);
    CHECK(q != NULL);
    if (!q) { _aligned_free(p); return; }
    CHECK_EQ_U((uintptr_t)q % 32, 0);
    for (i = 0; i < 40; i++) if (q[i] != (unsigned char)(i + 1)) { CHECK_EQ_I(q[i], i + 1); break; }
    for (i = m; i < 400; i++) if (q[i] != 0) { CHECK_EQ_I(q[i], 0); break; }
    _aligned_free(q);
}
int main(void) { tc_begin(); TC_RUN(null_acts_as_calloc); TC_RUN(grow_zero_tail); return tc_end(); }
