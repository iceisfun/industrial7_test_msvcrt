/* TC

 * id:    cpp_neg_exceptionptr_rethrow_empty_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: ?__ExceptionPtrRethrow@@YAXPEBX@Z ?__ExceptionPtrCreate@@YAXPEAX@Z
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   C++11 [propagation] (rethrow_exception requires a non-null exception_ptr)
 * note:  PROBE: rethrow_exception on a null exception_ptr is undefined in C++; MSVC is believed to throw std::bad_exception (a C++
 *        exception, SEH 0xE06D7363). Assertions are limited to: the child never returns normally and does not die by access
 *        violation. A vectored handler converts the C++ exception to exit code 100.
 */
#include "tc.h"
#include <errno.h>
typedef void (__cdecl *ep_1)(void *);
typedef void (__cdecl *ep_r)(const void *);
static unsigned char buf[64];

static LONG CALLBACK veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == 0xE06D7363u) ExitProcess(100);
    return EXCEPTION_CONTINUE_SEARCH;
}
TC_CASE(child_throws_or_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("rethrow", &code, 20000);
    CHECK(ran);
    CHECK(code != 51);
}
TC_CASE(child_not_av)
{
    DWORD code = 0; int ran = tc_spawn_self("rethrow", &code, 20000);
    CHECK(ran);
    CHECK(code != TC_STATUS_ACCESS_VIOLATION);
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        ep_1 cr = (ep_1)tc_crt_proc("?__ExceptionPtrCreate@@YAXPEAX@Z");
        ep_r rt = (ep_r)tc_crt_proc("?__ExceptionPtrRethrow@@YAXPEBX@Z");
        int i; if (!cr || !rt) return 78;
        for (i = 0; i < 64; i++) buf[i] = 0xCC;
        AddVectoredExceptionHandler(1, veh);
        cr(buf);
        rt(buf);
        return 51;
    }
    TC_RUN(child_throws_or_terminates); TC_RUN(child_not_av);
    return tc_end();
}
