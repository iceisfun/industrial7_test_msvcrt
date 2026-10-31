/* TC
 * id:    hp_neg_heapchk_after_failed_allocs
 * area:  heap
 * kind:  negative
 * funcs: _heapchk malloc calloc realloc free
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _heapchk (returns _HEAPOK / _HEAPEMPTY for a consistent heap); malloc/calloc/realloc return NULL on failure
 * note:  A barrage of impossible allocations must fail cleanly (NULL) and leave the heap consistent; a realloc failure leaves the
 *        original block valid.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <stdint.h>

static volatile size_t v_huge = SIZE_MAX / 2, v_one = 1;

TC_CASE(barrage)
{
    int i, r; void *keep = malloc(v_one + 63);
    unsigned char *k = (unsigned char *)keep;
    CHECK(keep != NULL);
    if (!k) return;
    for (i = 0; i < 64; i++) k[i] = (unsigned char)(i + 1);
    for (i = 0; i < 20; i++) {
        void *a = malloc(v_huge), *b = calloc(v_huge, 2), *c = realloc(keep, v_huge), *d;
        CHECK(a == NULL); CHECK(b == NULL); CHECK(c == NULL);
        if (a) free(a); if (b) free(b); if (c) { keep = c; k = (unsigned char *)keep; }
        d = malloc(v_one + 15);
        CHECK(d != NULL);
        free(d);
    }
    CHECK_EQ_I(k[0], 1); CHECK_EQ_I(k[63], 64);
    r = _heapchk();
    CHECK(r == _HEAPOK || r == _HEAPEMPTY);
    free(keep);
    r = _heapchk();
    CHECK(r == _HEAPOK || r == _HEAPEMPTY);
}
int main(void) { tc_begin(); TC_RUN(barrage); return tc_end(); }
