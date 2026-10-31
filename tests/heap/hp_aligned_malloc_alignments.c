/* TC
 * id:    hp_aligned_malloc_alignments
 * area:  heap
 * funcs: _aligned_malloc _aligned_free
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _aligned_malloc
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n, v_a;
static const size_t aligns[] = { 1, 2, 4, 8, 16, 32, 64, 128, 256, 4096, 65536 };

TC_CASE(alignments)
{
    size_t i, k;
    for (i = 0; i < sizeof aligns / sizeof aligns[0]; i++) {
        unsigned char *p, bad = 0;
        v_n = 100; v_a = aligns[i];
        p = (unsigned char *)_aligned_malloc(v_n, v_a);
        CHECK(p != NULL);
        if (!p) continue;
        CHECK_EQ_U((size_t)(uintptr_t)p % aligns[i], 0);
        for (k = 0; k < 100; k++) p[k] = (unsigned char)(k + 1);
        for (k = 0; k < 100; k++) if (p[k] != (unsigned char)(k + 1)) bad = 1;
        CHECK(!bad);
        _aligned_free(p);
    }
}
int main(void) { tc_begin(); TC_RUN(alignments); return tc_end(); }
