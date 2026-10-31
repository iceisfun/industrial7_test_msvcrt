/* TC
 * id:    cpp_neg_rttypeid_null_child
 * area:  cpp_runtime
 * kind:  negative
 * funcs: __RTtypeid
 * crt:   all
 * tier:  p1
 * conf:  inferred
 * ref:   C++ [expr.typeid]/2: typeid(*p) with null polymorphic p throws std::bad_typeid
 *        (the __RTtypeid export itself is an undocumented compiler-support helper; behaviour inferred from that rule)
 * note:  Only the fact of a C++ exception (code 0xE06D7363) with a non-NULL thrown object is asserted; message text and
 *        class identity are not. The child catches it with a vectored handler and exits 100 (object ok) / 101 (object NULL).
 */
#include "tc.h"

extern void *__cdecl __RTtypeid(void *);

static LONG WINAPI veh(PEXCEPTION_POINTERS ep)
{
    if (ep->ExceptionRecord->ExceptionCode == 0xE06D7363u) {
        if (ep->ExceptionRecord->NumberParameters >= 2 && ep->ExceptionRecord->ExceptionInformation[1] != 0)
            ExitProcess(100);
        ExitProcess(101);
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

static DWORD run_child(void) { DWORD code = 0; int ran = tc_spawn_self("typeid_null", &code, 20000); return ran ? code : 0xdead0003; }

TC_CASE(throws_cxx_exception) { DWORD c = run_child(); CHECK(c == 100 || c == 101); }
TC_CASE(does_not_return)      { DWORD c = run_child(); CHECK(c != 52); CHECK(c != 0xdead0003); }
TC_CASE(thrown_object_is_exception) { CHECK_EQ_U(run_child(), 100); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        volatile void *p = NULL;
        AddVectoredExceptionHandler(1, veh);
        __RTtypeid((void *)p);
        return 52;  /* reached only if nothing was thrown */
    }
    TC_RUN(throws_cxx_exception); TC_RUN(does_not_return); TC_RUN(thrown_object_is_exception);
    return tc_end();
}
