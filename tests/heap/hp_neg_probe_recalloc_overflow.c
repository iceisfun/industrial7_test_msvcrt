/* TC
 * id:    hp_neg_probe_recalloc_overflow
 * area:  heap
 * funcs: _recalloc malloc free
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _recalloc (by analogy with calloc)
 * note:  PROBING that _recalloc(p,n,s) with n*s overflowing size_t returns NULL and leaves p valid.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n, v_s;

TC_CASE(overflow_keeps_block)
{
    char *p, *q; v_n = 8; p = (char *)malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    p[0] = 'x';
    v_n = ((size_t)1 << (sizeof(size_t) * 8 - 1)) + 1; v_s = 2;
    q = (char *)_recalloc(p, v_n, v_s);
    CHECK(q == NULL);
    if (q) { free(q); return; }
    CHECK_EQ_I(p[0], 'x');
    free(p);
}
int main(void) { tc_begin(); TC_RUN(overflow_keeps_block); return tc_end(); }
