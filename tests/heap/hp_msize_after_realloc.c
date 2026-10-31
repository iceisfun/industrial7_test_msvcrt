/* TC
 * id:    hp_msize_after_realloc
 * area:  heap
 * funcs: _msize realloc malloc free
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _msize, realloc
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>
#include <errno.h>
#include <string.h>

static volatile size_t v_n;

TC_CASE(grow)
{
    void *p, *q;
    v_n = 16; p = malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    v_n = 5000; q = realloc(p, v_n); CHECK(q != NULL);
    if (!q) { free(p); return; }
    CHECK(_msize(q) >= 5000);
    free(q);
}
TC_CASE(shrink)
{
    void *p, *q; size_t old;
    v_n = 5000; p = malloc(v_n); CHECK(p != NULL);
    if (!p) return;
    old = _msize(p);
    v_n = 100; q = realloc(p, v_n); CHECK(q != NULL);
    if (!q) { free(p); return; }
    CHECK(_msize(q) >= 100);
    CHECK(_msize(q) <= old);
    free(q);
}
int main(void) { tc_begin(); TC_RUN(grow); TC_RUN(shrink); return tc_end(); }
