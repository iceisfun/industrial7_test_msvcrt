/* TC
 * id:    cpp_neg_exception_what_null_this_probe_child
 * area:  cpp_runtime
 * funcs:
 * procs: ?what@exception@std@@UEBAPEBDXZ
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C++ [exception]: calling a member on a null object is undefined; the CRT performs no null check
 * note:  PROBE (child process): std::exception::what() with this==NULL reads _Mywhat at this+8, so the child is expected to take
 *        STATUS_ACCESS_VIOLATION (0xC0000005). A vectored handler exits 100 on AV; exit 52 means the call returned.
 */
#include "tc.h"

typedef const char *(__cdecl *what_t)(const void *);

static LONG WINAPI veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == TC_STATUS_ACCESS_VIOLATION) ExitProcess(100);
    return EXCEPTION_CONTINUE_SEARCH;
}
static DWORD run_child(void) { DWORD code = 0; int ran = tc_spawn_self("what_null", &code, 20000); return ran ? code : 0xdead0003; }

TC_CASE(what_null_this_av) { DWORD c = run_child(); CHECK_EQ_U(c, 100); CHECK(c != 0 && c != 77); }
TC_CASE(what_null_this_not_returns) { DWORD c = run_child(); CHECK(c != 52); CHECK(c != 0xdead0003); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        what_t w = (what_t)tc_crt_proc("?what@exception@std@@UEBAPEBDXZ");
        const void *volatile p = NULL;
        if (!w) return 77;
        AddVectoredExceptionHandler(1, veh);
        (void)w((const void *)p);
        return 52;  /* reached only if the call returned */
    }
    TC_RUN(what_null_this_av); TC_RUN(what_null_this_not_returns);
    return tc_end();
}
