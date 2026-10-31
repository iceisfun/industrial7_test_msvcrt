/* TC
 * id:    cpp_neg_typeinfo_eq_null_arg_probe_child
 * area:  cpp_runtime
 * funcs:
 * procs: ??8type_info@@QEBA_NAEBV0@@Z
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C++ [dcl.ref]: a reference cannot be null; the CRT's operator== has no null guard
 * note:  PROBE (child process): type_info::operator==(*NULL) compares names through the argument and is expected to fault with an
 *        access violation. A vectored handler exits 100 on 0xC0000005; exit 52 means the call returned.
 */
#include "tc.h"

struct fti { void *vptr; void *data; char name[16]; };
typedef unsigned char (__cdecl *eq_t)(const void *, const void *);

static LONG WINAPI veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == TC_STATUS_ACCESS_VIOLATION) ExitProcess(100);
    return EXCEPTION_CONTINUE_SEARCH;
}
static DWORD run_child(void) { DWORD code = 0; int ran = tc_spawn_self("eq_null", &code, 20000); return ran ? code : 0xdead0003; }

TC_CASE(eq_null_ref_av) { DWORD c = run_child(); CHECK_EQ_U(c, 100); CHECK(c != 0 && c != 77); }
TC_CASE(eq_null_ref_not_returns) { DWORD c = run_child(); CHECK(c != 52); CHECK(c != 0xdead0003); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        static struct fti t = { NULL, NULL, ".H" };
        eq_t e = (eq_t)tc_crt_proc("??8type_info@@QEBA_NAEBV0@@Z");
        const void *volatile p = NULL;
        if (!e) return 77;
        AddVectoredExceptionHandler(1, veh);
        (void)e(&t, (const void *)p);
        return 52;
    }
    TC_RUN(eq_null_ref_av); TC_RUN(eq_null_ref_not_returns);
    return tc_end();
}
