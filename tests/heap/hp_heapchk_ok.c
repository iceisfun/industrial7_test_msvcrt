/* TC
 * id:    hp_heapchk_ok
 * area:  heap
 * funcs: _heapchk malloc free realloc
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _heapchk (returns _HEAPOK, _HEAPEMPTY, _HEAPBADBEGIN, _HEAPBADNODE, _HEAPBADPTR, _HEAPEND)
 * note:  A healthy heap must report _HEAPOK (-2) or _HEAPEMPTY (-1); never a BAD* value.
 */
#include "tc.h"
#include <malloc.h>

#define HOK(r) ((r) == _HEAPOK || (r) == _HEAPEMPTY)
static volatile size_t v_n = 64;

TC_CASE(fresh) { int r = _heapchk(); CHECK(HOK(r)); }
TC_CASE(after_alloc_free)
{
    void *p[50]; int i, r;
    for (i = 0; i < 50; i++) p[i] = malloc(v_n * (i + 1));
    r = _heapchk(); CHECK(HOK(r));
    for (i = 0; i < 50; i += 2) free(p[i]);
    r = _heapchk(); CHECK(HOK(r));
    for (i = 1; i < 50; i += 2) free(p[i]);
    r = _heapchk(); CHECK(HOK(r));
}
TC_CASE(after_realloc_chain)
{
    char *p = (char *)malloc(8); size_t n; int r;
    CHECK(p != NULL); if (!p) return;
    for (n = 16; n <= 65536; n *= 2) { char *q = (char *)realloc(p, n); CHECK(q != NULL); if (!q) break; p = q; p[n - 1] = 1; }
    r = _heapchk(); CHECK(HOK(r));
    free(p);
    r = _heapchk(); CHECK(HOK(r));
}
int main(void) { tc_begin(); TC_RUN(fresh); TC_RUN(after_alloc_free); TC_RUN(after_realloc_chain); return tc_end(); }
