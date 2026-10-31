/* TC
 * id:    hp_neg_heapmin_after_heap_use_not_negative
 * area:  heap
 * kind:  negative
 * funcs: _heapmin _heapchk malloc free
 * crt:   all
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _heapchk (returns _HEAPOK/_HEAPEMPTY on a consistent heap), _heapmin (returns 0 on success, -1 on failure)
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_n = 100;

TC_CASE(heapchk_not_corrupt)
{
    void *a = malloc(v_n), *b = malloc(v_n * 50);
    int r = _heapchk();
    CHECK(r != _HEAPBADBEGIN);
    CHECK(r != _HEAPBADNODE);
    CHECK(r != _HEAPBADPTR);
    CHECK(r == _HEAPOK || r == _HEAPEMPTY);
    free(a); free(b);
    r = _heapchk();
    CHECK(r == _HEAPOK || r == _HEAPEMPTY);
}
TC_CASE(heapmin_not_failure)
{
    void *a = malloc(v_n * 100); int r;
    free(a);
    r = _heapmin();
    CHECK(r != -1);
    CHECK(_heapchk() == _HEAPOK || _heapchk() == _HEAPEMPTY);
}
int main(void) { tc_begin(); TC_RUN(heapchk_not_corrupt); TC_RUN(heapmin_not_failure); return tc_end(); }
