/* TC

 * id:    cpp_exceptionptr_empty
 * area:  cpp_runtime
 * funcs:
 * procs: ?__ExceptionPtrCreate@@YAXPEAX@Z ?__ExceptionPtrDestroy@@YAXPEAX@Z ?__ExceptionPtrCopy@@YAXPEAXPEBX@Z ?__ExceptionPtrAssign@@YAXPEAXPEBX@Z ?__ExceptionPtrCompare@@YA_NPEBX0@Z ?__ExceptionPtrToBool@@YA_NPEBX@Z ?__ExceptionPtrSwap@@YAXPEAX0@Z
 * crt:   msvcr120 msvcr110
 * tier:  p1
 * conf:  observed
 * ref:   C++11 [propagation] (default-constructed exception_ptr is null, equal to another null, explicit bool false)
 * note:  These __ExceptionPtr* exports are the undocumented ABI behind the std::exception_ptr inline members in <exception>; semantics
 *        (null == empty) follow C++11 [propagation].
 *         Storage size varies by CRT, so 64-byte buffers filled with 0xCC are used. Swap is only exercised on msvcr120 (skipped elsewhere; run last). msvcr100 is omitted: its export table lacks __ExceptionPtrToBool (exception_ptr::operator bool is inline there).
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

TC_CASE(create_is_false)
{
    fill(b1);
    p_create(b1);
    CHECK(!p_tobool(b1));
    p_destroy(b1);
}
TC_CASE(empty_equal)
{
    fill(b1); fill(b2);
    p_create(b1); p_create(b2);
    CHECK(p_cmp(b1, b2) != 0);
    p_destroy(b1); p_destroy(b2);
}
TC_CASE(copy_empty)
{
    fill(b1); fill(b2);
    p_create(b1);
    p_copy(b2, b1);
    CHECK(!p_tobool(b2));
    CHECK(p_cmp(b1, b2) != 0);
    p_destroy(b1); p_destroy(b2);
}
TC_CASE(assign_empty)
{
    fill(b1); fill(b2);
    p_create(b1); p_create(b2);
    p_assign(b2, b1);
    CHECK(!p_tobool(b2));
    CHECK(p_cmp(b1, b2) != 0);
    p_destroy(b1); p_destroy(b2);
}
TC_CASE(swap_empty)
{
    if (!TC_AT_LEAST(120)) TC_SKIP("Swap only in msvcr120");
    CHECK(p_swap != NULL);
    if (!p_swap) return;
    fill(b1); fill(b2);
    p_create(b1); p_create(b2);
    p_swap(b1, b2);
    CHECK(!p_tobool(b1)); CHECK(!p_tobool(b2));
    p_destroy(b1); p_destroy(b2);
}
TC_CASE(destroy_ok)
{
    fill(b1);
    p_create(b1);
    p_destroy(b1);
    CHECK(1);
}
int main(void)
{
    tc_begin();
    load();
    if (!p_create || !p_destroy || !p_copy || !p_assign || !p_cmp || !p_tobool) {
        tc_write("TC FAIL cpp_exceptionptr_empty::__missing exports\n"); return 1; }
    TC_RUN(create_is_false); TC_RUN(empty_equal); TC_RUN(copy_empty); TC_RUN(assign_empty); TC_RUN(destroy_ok); TC_RUN(swap_empty);
    return tc_end();
}
