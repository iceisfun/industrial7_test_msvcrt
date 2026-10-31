/* TC
 * id:    hp_neg_freea_foreign_pointer_probe
 * area:  heap
 * kind:  negative
 * funcs:
 * procs: _freea
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _freea (only frees blocks _malloca placed on the heap, identified by a marker)
 * note:  _malloca is a header macro (not exported), so the marker-protocol case builds the marked blocks by hand using the public
 *        _ALLOCA_S_* constants from malloc.h. PROBING survivability: _freea on a plain malloc'd pointer (no marker) must return without crashing; the block is deliberately
 *        leaked because whether it is freed cannot be asserted. Skips where _freea is not exported.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

typedef void (__cdecl *freea_t)(void *);
static volatile size_t v_n = 16;

TC_CASE(foreign_pointer_survives)
{
    freea_t f = (freea_t)tc_crt_proc("_freea");
    void *p;
    if (!f) TC_SKIP("_freea not exported");
    p = malloc(v_n);
    CHECK(p != NULL);
    f(p);                 /* must not crash; block intentionally leaked */
    f(NULL);              /* NULL is a documented no-op */
    CHECK(1);
}
TC_CASE(marked_blocks)
{
    freea_t f = (freea_t)tc_crt_proc("_freea");
    char stackbuf[64]; char *h;
    if (!f) TC_SKIP("_freea not exported");
    /* heap-marked block, as the public _malloca macro would produce: _freea must release it (survive) */
    h = (char *)malloc(v_n + _ALLOCA_S_MARKER_SIZE);
    CHECK(h != NULL);
    if (h) { *(int *)h = _ALLOCA_S_HEAP_MARKER; f(h + _ALLOCA_S_MARKER_SIZE); }
    /* stack-marked block: _freea must leave it alone (survive) */
    *(int *)stackbuf = _ALLOCA_S_STACK_MARKER;
    f(stackbuf + _ALLOCA_S_MARKER_SIZE);
    CHECK(1);
}
int main(void) { tc_begin(); TC_RUN(foreign_pointer_survives); TC_RUN(marked_blocks); return tc_end(); }
