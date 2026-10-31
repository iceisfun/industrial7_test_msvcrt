/* TC
 * id:    hp_neg_recalloc_overflow
 * area:  heap
 * funcs: _recalloc malloc free
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _recalloc (ENOMEM)
 * note:  PROBING that _recalloc rejects n*size overflow with NULL+ENOMEM and keeps the original block; docs do not spell out the overflow check.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <stdint.h>

static volatile size_t v_a, v_b;

TC_CASE(null_overflow) { void *p; errno = 0; v_a = SIZE_MAX; v_b = 2; p = _recalloc(NULL, v_a, v_b); CHECK(p == NULL); CHECK_EQ_I(errno, ENOMEM); if (p) free(p); }
TC_CASE(overflow_keeps_block)
{
    unsigned char *p, *q; int i, bad = 0;
    p = (unsigned char *)malloc(32); CHECK(p != NULL); if (!p) return;
    for (i = 0; i < 32; i++) p[i] = (unsigned char)(i + 3);
    errno = 0; v_a = SIZE_MAX / 2 + 1; v_b = 2; q = (unsigned char *)_recalloc(p, v_a, v_b);
    CHECK(q == NULL); CHECK_EQ_I(errno, ENOMEM);
    if (q) p = q;
    for (i = 0; i < 32; i++) if (p[i] != (unsigned char)(i + 3)) bad++;
    CHECK_EQ_I(bad, 0);
    errno = 0; v_a = 1; v_b = SIZE_MAX; q = (unsigned char *)_recalloc(p, v_a, v_b);
    CHECK(q == NULL); CHECK_EQ_I(errno, ENOMEM);
    if (q) p = q;
    free(p);
}
int main(void) { tc_begin(); TC_RUN(null_overflow); TC_RUN(overflow_keeps_block); return tc_end(); }
