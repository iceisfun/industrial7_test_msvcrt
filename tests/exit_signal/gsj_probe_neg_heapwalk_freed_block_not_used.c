/* TC
 * id:    gsj_probe_neg_heapwalk_freed_block_not_used
 * area:  exit_signal
 * kind:  negative
 * funcs: _heapwalk malloc free
 * procs:
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _heapwalk
 * note:  PROBING that after free() a block is no longer reported as _USEDENTRY by _heapwalk. Used side is hp_probe_heapwalk_finds_block.
 *        Terminating status is a separate case because the ucrt heap is the (noisy) process heap.
 */
#include "tc.h"
#include <malloc.h>
#include <stdlib.h>

static volatile size_t v_n = 777;
static void *g_p;

/* returns 1 if USED entry for p found; *last gets final walk status, *size the entry size */
static int walk(void *p, int *last, size_t *size)
{
    _HEAPINFO e; int r, n = 0, found = 0;
    e._pentry = NULL;
    while ((r = _heapwalk(&e)) == _HEAPOK && n < 1000000) {
        n++;
        if (e._useflag == _USEDENTRY && e._pentry == p) { found = 1; if (size) *size = e._size; }
    }
    if (n >= 1000000) r = -99;
    *last = r; return found;
}
TC_CASE(used_while_allocated)
{
    int last; size_t sz = 0; g_p = malloc(v_n); CHECK(g_p != NULL);
    CHECK(walk(g_p, &last, &sz));
    CHECK(sz >= 777);
}
TC_CASE(not_used_after_free)
{
    int last; if (!g_p) TC_SKIP("no block");
    free(g_p);
    CHECK(!walk(g_p, &last, NULL));
    g_p = NULL;
}
TC_CASE(walk_terminates_cleanly)
{
    int last; void *p = malloc(v_n);
    walk(p, &last, NULL);
    CHECK(last == _HEAPEND || last == _HEAPEMPTY);
    free(p);
}
int main(void)
{
    tc_begin();
    TC_RUN(used_while_allocated); TC_RUN(not_used_after_free); TC_RUN(walk_terminates_cleanly);
    return tc_end();
}
