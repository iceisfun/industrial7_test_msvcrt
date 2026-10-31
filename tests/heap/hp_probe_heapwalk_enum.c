/* TC
 * id:    hp_probe_heapwalk_enum
 * area:  heap
 * funcs: _heapwalk malloc free
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _heapwalk
 * note:  PROBING _heapwalk: first call with _pentry==NULL returns _HEAPOK or _HEAPEMPTY, and iterating ends with
 *        _HEAPEND. The ucrt docs describe _heapwalk as limited, so the exact results are what is being measured.
 *        Loop is capped at 1,000,000 iterations.
 */
#include "tc.h"
#include <malloc.h>

static volatile size_t v_n = 128;

TC_CASE(first_call)
{
    _HEAPINFO e; int r, n = 0; void *p = malloc(v_n);
    e._pentry = NULL;
    r = _heapwalk(&e);
    CHECK(r == _HEAPOK || r == _HEAPEMPTY);
    /* drain so that a CRT which locks the heap for the duration of a walk releases it */
    while (r == _HEAPOK && n < 1000000) { r = _heapwalk(&e); n++; }
    free(p);
}
TC_CASE(loop_ends_with_heapend)
{
    _HEAPINFO e; int r, n = 0; void *p = malloc(v_n);
    e._pentry = NULL;
    while ((r = _heapwalk(&e)) == _HEAPOK && n < 1000000) n++;
    CHECK_EQ_I(r, _HEAPEND);
    CHECK(n < 1000000);
    free(p);
}
int main(void) { tc_begin(); TC_RUN(first_call); TC_RUN(loop_ends_with_heapend); return tc_end(); }
