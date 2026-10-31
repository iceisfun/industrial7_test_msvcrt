/* TC

 * id:    cpp_exceptionptr_current_none
 * area:  cpp_runtime
 * funcs:
 * procs: ?__ExceptionPtrCurrentException@@YAXPEAX@Z ?__ExceptionPtrCreate@@YAXPEAX@Z ?__ExceptionPtrToBool@@YA_NPEBX@Z ?__ExceptionPtrDestroy@@YAXPEAX@Z
 * crt:   msvcr120 msvcr110
 * tier:  p1
 * conf:  observed
 * ref:   C++11 18.8.5 current_exception (null exception_ptr if no exception is being handled)
 * note:  Called from plain C, so no exception is being handled. msvcr100 omitted (no __ExceptionPtrToBool export).
 */
#include "tc.h"
#include <errno.h>
typedef void (__cdecl *ep_1)(void *);
typedef void (__cdecl *ep_2)(void *, const void *);
typedef unsigned char (__cdecl *ep_cmp)(const void *, const void *);
typedef unsigned char (__cdecl *ep_bool)(const void *);
typedef void (__cdecl *ep_swap)(void *, void *);
static ep_1 p_create, p_destroy, p_cur;
static ep_2 p_copy, p_assign;
static ep_cmp p_cmp;
static ep_bool p_tobool;
static ep_swap p_swap;
static unsigned char b1[64], b2[64];
static void fill(unsigned char *b) { int i; for (i = 0; i < 64; i++) b[i] = 0xCC; }
static void load(void)
{
    p_create = (ep_1)tc_crt_proc("?__ExceptionPtrCreate@@YAXPEAX@Z");
    p_destroy = (ep_1)tc_crt_proc("?__ExceptionPtrDestroy@@YAXPEAX@Z");
    p_cur = (ep_1)tc_crt_proc("?__ExceptionPtrCurrentException@@YAXPEAX@Z");
    p_copy = (ep_2)tc_crt_proc("?__ExceptionPtrCopy@@YAXPEAXPEBX@Z");
    p_assign = (ep_2)tc_crt_proc("?__ExceptionPtrAssign@@YAXPEAXPEBX@Z");
    p_cmp = (ep_cmp)tc_crt_proc("?__ExceptionPtrCompare@@YA_NPEBX0@Z");
    p_tobool = (ep_bool)tc_crt_proc("?__ExceptionPtrToBool@@YA_NPEBX@Z");
    p_swap = (ep_swap)tc_crt_proc("?__ExceptionPtrSwap@@YAXPEAX0@Z");
}

TC_CASE(none_is_empty)
{
    fill(b1);
    p_cur(b1);
    CHECK(!p_tobool(b1));
    p_destroy(b1);
}
TC_CASE(twice_ok)
{
    fill(b1); fill(b2);
    p_cur(b1); p_cur(b2);
    CHECK(!p_tobool(b1)); CHECK(!p_tobool(b2));
    p_destroy(b1); p_destroy(b2);
}
TC_CASE(destroy_after)
{
    fill(b1);
    p_create(b1); p_cur(b1);
    p_destroy(b1);
    CHECK(1);
}
int main(void)
{
    tc_begin();
    load();
    if (!p_create || !p_destroy || !p_cur || !p_tobool) { tc_write("TC FAIL cpp_exceptionptr_current_none::__missing exports\n"); return 1; }
    TC_RUN(none_is_empty); TC_RUN(twice_ok); TC_RUN(destroy_after);
    return tc_end();
}
