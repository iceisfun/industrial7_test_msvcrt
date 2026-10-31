/* TC
 * id:    hp_expand_shrink
 * area:  heap
 * funcs: _expand malloc _msize free
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  inferred
 * ref:   MS docs _expand
 * note:  Docs: returns pointer to the resized block (same address) or NULL on failure; docs do not explicitly promise that a shrink succeeds, but in-place shrink is the
 *        overwhelmingly expected result (nothing needs to move).
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(shrink_in_place)
{
    unsigned char *p, *q; size_t i, bad = 0;
    v_n = 200; p = (unsigned char *)malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    for (i = 0; i < 200; i++) p[i] = (unsigned char)(i ^ 0x35);
    v_n = 100; q = (unsigned char *)_expand(p, v_n);
    CHECK_EQ_P(q, p);
    if (!q) { free(p); return; }
    for (i = 0; i < 100; i++) bad += q[i] != (unsigned char)(i ^ 0x35);
    CHECK_EQ_U(bad, 0);
    CHECK(_msize(q) >= 100);
    free(q);
}
int main(void) { tc_begin(); TC_RUN(shrink_in_place); return tc_end(); }
