/* TC
 * id:    ucr_get_heap_handle_probe
 * area:  ucrt_only
 * funcs:
 * procs: _get_heap_handle
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   ucrtbase export (undocumented)
 * note:  PROBE: handle is non-NULL and a valid heap; equality with GetProcessHeap() is probed in its own case.
 */
#include "tc.h"

typedef intptr_t (__cdecl *gh_t)(void);
TC_CASE(valid)
{
    gh_t f = (gh_t)tc_crt_proc("_get_heap_handle");
    HANDLE h;
    CHECK(f != NULL); if (!f) return;
    h = (HANDLE)f();
    CHECK(h != NULL);
    CHECK(HeapValidate(h, 0, NULL));
}
TC_CASE(is_process_heap)
{
    gh_t f = (gh_t)tc_crt_proc("_get_heap_handle");
    if (!f) TC_SKIP("not exported");
    CHECK_EQ_P((HANDLE)f(), GetProcessHeap());
}
int main(void) { tc_begin(); TC_RUN(valid); TC_RUN(is_process_heap); return tc_end(); }
