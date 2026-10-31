/* TC
 * id:    hp_probe_heapwalk_finds_block
 * area:  heap
 * funcs: _heapwalk malloc free
 * crt:   <140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _heapwalk (UCRT excluded: heap walking is documented as unsupported there)
 * note:  PROBING that the msvcrt-era _heapwalk (small-block heap / HeapWalk based) enumerates a live malloc'd block as a
 *        _USEDENTRY with _pentry == the pointer and _size >= requested size. Bounded walk, no assertion on walk status.
 */
#include "tc.h"
#include <malloc.h>

TC_CASE(finds_live_block)
{
    char *volatile p = (char *)malloc(300);
    _HEAPINFO hi;
    int found = 0, n = 0, st;
    CHECK(p != NULL);
    if (!p) return;
    hi._pentry = NULL;
    while ((st = _heapwalk(&hi)) == _HEAPOK && n < 2000000) {
        n++;
        if (hi._useflag == _USEDENTRY && (void *)hi._pentry == (void *)p) {
            found = 1;
            CHECK(hi._size >= 300);
            break;
        }
    }
    CHECK(found);
    free(p);
}

int main(void) { tc_begin(); TC_RUN(finds_live_block); return tc_end(); }
