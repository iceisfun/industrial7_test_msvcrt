/* TC
 * id:    gsj_free_base_interop
 * area:  exit_signal
 * funcs: malloc free _heapchk
 * procs: _malloc_base _free_base _calloc_base
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   none (ucrt internal exports _malloc_base/_free_base/_calloc_base)
 * note:  PROBING that the _base allocators share the heap with malloc/free so blocks can be released by either side.
 *        Not documented; kept separate from documented allocator tests.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

TC_DECL_PROC(void *, _malloc_base, (size_t));
TC_DECL_PROC(void *, _calloc_base, (size_t, size_t));
TC_DECL_PROC(void, _free_base, (void *));
static volatile size_t v_n = 64;

TC_CASE(malloc_freed_by_free_base)
{
    void *p;
    if (!tc_p__free_base) TC_SKIP("no _free_base");
    p = malloc(v_n); CHECK(p != NULL);
    tc_p__free_base(p);
}
TC_CASE(malloc_base_freed_by_free)
{
    void *p;
    if (!tc_p__malloc_base) TC_SKIP("no _malloc_base");
    p = tc_p__malloc_base(v_n); CHECK(p != NULL);
    free(p);
}
TC_CASE(calloc_base_freed_by_free_heap_ok)
{
    void *p;
    if (!tc_p__calloc_base) TC_SKIP("no _calloc_base");
    p = tc_p__calloc_base(4, v_n); CHECK(p != NULL);
    free(p);
    CHECK_EQ_I(_heapchk(), _HEAPOK);
}
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_malloc_base); TC_LOAD_PROC(_calloc_base); TC_LOAD_PROC(_free_base);
    TC_RUN(malloc_freed_by_free_base); TC_RUN(malloc_base_freed_by_free); TC_RUN(calloc_base_freed_by_free_heap_ok);
    return tc_end();
}
