/* TC
 * id:    exp_heap_misc_exported
 * area:  exports
 * funcs: 
 * procs: _heapchk _heapmin _heapwalk _get_heap_handle
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _heapchk, _heapmin, _heapwalk, _get_heap_handle
 * note:  Export presence only (plus _get_heap_handle returns non-NULL). _get_heap_handle first appears in the VS2005 CRT; msvcr80 listed per its documentation.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>
#include <math.h>
#include <float.h>
#include <malloc.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define HASNOT(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)


TC_CASE(heapchk)
{
    HAS("_heapchk");
}

TC_CASE(heapmin)
{
    HAS("_heapmin");
}

TC_CASE(heapwalk)
{
    HAS("_heapwalk");
}

TC_CASE(get_heap_handle_nonnull_when_called)
{
    intptr_t (__cdecl *gh)(void) = (intptr_t (__cdecl *)(void))tc_crt_proc("_get_heap_handle");
    HAS("_get_heap_handle");
    if (!gh) TC_SKIP("_get_heap_handle not exported");
    CHECK(gh() != 0);
}

int main(void)
{
    tc_begin();
    TC_RUN(heapchk);
    TC_RUN(heapmin);
    TC_RUN(heapwalk);
    TC_RUN(get_heap_handle_nonnull_when_called);
    return tc_end();
}
