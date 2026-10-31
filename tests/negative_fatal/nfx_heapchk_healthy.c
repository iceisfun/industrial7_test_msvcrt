/* TC
 * id:    nfx_heapchk_healthy
 * area:  negative_fatal
 * funcs: _heapchk malloc free
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _heapchk (returns _HEAPOK, _HEAPEMPTY, ... ; _HEAPOK=-2, _HEAPEMPTY=-1)
 */
#include "tc.h"
#include <malloc.h>
#include <stdlib.h>

TC_CASE(fresh)
{
    int r = _heapchk();
    CHECK(r == -2 || r == -1);
}
TC_CASE(after_alloc_free)
{
    void *volatile p[6]; int i, r;
    for (i = 0; i < 6; i++) p[i] = malloc((size_t)(16 << i));
    for (i = 0; i < 6; i++) CHECK(p[i] != NULL);
    r = _heapchk(); CHECK(r == -2 || r == -1);
    for (i = 0; i < 6; i += 2) free(p[i]);
    r = _heapchk(); CHECK(r == -2 || r == -1);
    for (i = 1; i < 6; i += 2) free(p[i]);
    r = _heapchk(); CHECK(r == -2 || r == -1);
}
int main(void)
{
    tc_begin();
    TC_RUN(fresh); TC_RUN(after_alloc_free);
    return tc_end();
}
