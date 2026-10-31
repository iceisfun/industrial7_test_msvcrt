/* TC

 * id:    cpp_neg_exceptionptr_rethrow_empty_code_probe_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs:
 * procs: ?__ExceptionPtrRethrow@@YAXPEBX@Z ?__ExceptionPtrCreate@@YAXPEAX@Z
 * crt:   msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   C++11 [propagation]; MSVC STL rethrow_exception
 * note:  PROBE: rethrow of a null exception_ptr raises an MSVC C++ exception (SEH 0xE06D7363), observed via a vectored handler that
 *        exits with code 100. Separate from the non-probe-style "never returns / not AV" checks.
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
TC_CASE(raises_msc_exception)
{
    DWORD code = 0; int ran = tc_spawn_self("rethrow", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 100);
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
    TC_RUN(raises_msc_exception);
    return tc_end();
}
