/* TC
 * id:    hp_probe_aligned_realloc_null
 * area:  heap
 * funcs: _aligned_realloc _aligned_free
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _aligned_realloc
 * note:  PROBING: _aligned_realloc(NULL, n, align) behaves like _aligned_malloc(n, align) (non-NULL, aligned,
 *        usable). By analogy with realloc(NULL, n); not stated in the _aligned_realloc docs.
 */
#include "tc.h"
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_a = 64;

TC_CASE(null_acts_as_malloc)
{
    unsigned char *p = (unsigned char *)_aligned_realloc(NULL, 100, v_a);
    size_t i;
    CHECK(p != NULL); if (!p) return;
    CHECK_EQ_U((uintptr_t)p % 64, 0);
    for (i = 0; i < 100; i++) p[i] = (unsigned char)i;
    for (i = 0; i < 100; i++) if (p[i] != (unsigned char)i) { CHECK_EQ_I(p[i], i); break; }
    _aligned_free(p);
}
int main(void) { tc_begin(); TC_RUN(null_acts_as_malloc); return tc_end(); }
