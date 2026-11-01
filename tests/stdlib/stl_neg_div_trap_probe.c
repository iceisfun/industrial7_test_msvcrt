/* TC
 * id:    stl_neg_div_trap_probe
 * area:  stdlib
 * funcs: div ldiv
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.6.2 (division by zero / unrepresentable quotient: behaviour undefined)
 * note:  Probes that on Windows x86/x64 div(n,0) raises a hardware divide exception which is unhandled in the child: exit
 *        status 0xC0000094 (INTEGER_DIVIDE_BY_ZERO). Class E: div(INT_MIN,-1) (expected 0xC0000094 or 0xC0000095) is wrapped in
 *        TC_HAZARD because on the Win10 22H2 reference VM the child hangs for div, ldiv and a plain volatile INT_MIN / -1 even
 *        with an unhandled-exception filter installed (5 s timeouts, three runs); whether bare-metal Windows hangs too is
 *        unknown. Fatal paths run in child processes.
 */
#include "tc.h"
#include <stdlib.h>
#include <limits.h>

static LONG WINAPI tc_uef(EXCEPTION_POINTERS *e) { TerminateProcess(GetCurrentProcess(), e->ExceptionRecord->ExceptionCode); return EXCEPTION_EXECUTE_HANDLER; }

TC_CASE(div_by_zero)
{
    DWORD code = 0; int ran = tc_spawn_self("div0", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000094u);
}
TC_CASE(ldiv_by_zero)
{
    DWORD code = 0; int ran = tc_spawn_self("ldiv0", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 0xC0000094u);
}
TC_CASE(div_min_by_m1)
{
    DWORD code = 0; int ran;
    TC_HAZARD("INT_MIN / -1 hangs the child on the Win10 22H2 reference VM: div, ldiv and a plain volatile / all ran into the 5 s timeout even with an unhandled-exception filter installed");
    ran = tc_spawn_self("divmin", &code, 5000);
    CHECK(ran); CHECK(code == 0xC0000094u || code == 0xC0000095u);
}
int main(void)
{
    const char *c; volatile int z = 0, m = INT_MIN, n = -1; volatile long lz = 0;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        div_t r; ldiv_t lr;
        if (c[0] == 'd' && c[3] == '0') { r = div(5, z); return r.quot ? 77 : 78; }
        if (c[0] == 'l') { lr = ldiv(5L, lz); return lr.quot ? 77 : 78; }
        SetUnhandledExceptionFilter(tc_uef);
        r = div(m, n); return r.quot ? 77 : 78;
    }
    TC_RUN(div_by_zero); TC_RUN(ldiv_by_zero); TC_RUN(div_min_by_m1);
    return tc_end();
}
