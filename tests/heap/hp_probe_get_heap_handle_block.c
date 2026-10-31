/* TC
 * id:    hp_probe_get_heap_handle_block
 * area:  heap
 * funcs: _get_heap_handle malloc free
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _get_heap_handle
 * note:  PROBING: a malloc'd block is a HeapAlloc block of the heap returned by _get_heap_handle (HeapValidate on
 *        the block succeeds, HeapSize >= requested). Expected from VS2010+ where the SBH was removed; an
 *        implementation detail, not documented.
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_n = 100;

TC_CASE(validate_block)
{
    HANDLE h = (HANDLE)_get_heap_handle(); void *p = malloc(v_n);
    CHECK(p != NULL); if (!p || !h) { free(p); return; }
    CHECK(HeapValidate(h, 0, p));
    free(p);
}
TC_CASE(heapsize)
{
    HANDLE h = (HANDLE)_get_heap_handle(); void *p = malloc(v_n);
    CHECK(p != NULL); if (!p || !h) { free(p); return; }
    CHECK(HeapSize(h, 0, p) >= 100);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(validate_block); TC_RUN(heapsize); return tc_end(); }
