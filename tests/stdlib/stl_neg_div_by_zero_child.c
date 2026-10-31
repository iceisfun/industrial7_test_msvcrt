/* TC
 * id:    stl_neg_div_by_zero_child
 * area:  stdlib
 * funcs: div ldiv
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs div (zero denominator terminates the program; exact code 0xC0000094 is observed x86 idiv #DE behaviour); C11 7.22.6.2
 * note:  Fatal path in child processes. Integer division by zero in div/ldiv executes idiv and raises EXCEPTION_INT_DIVIDE_BY_ZERO (0xC0000094). Child 1 leaves it unhandled; child 2 uses an unhandled-exception filter to prove the exception code.
 */
#include "tc.h"
#include <stdlib.h>

static volatile int a = 1, z = 0; static volatile long la = 1, lz = 0;

static LONG WINAPI filt(EXCEPTION_POINTERS *ep)
{
    if (ep->ExceptionRecord->ExceptionCode == 0xC0000094u) ExitProcess(100);
    ExitProcess(101);
    return EXCEPTION_EXECUTE_HANDLER;
}

TC_CASE(div_unhandled) { DWORD c = 0; CHECK(tc_spawn_self("div0", &c, 20000)); CHECK_EQ_U(c, 0xC0000094u); }
TC_CASE(ldiv_unhandled) { DWORD c = 0; CHECK(tc_spawn_self("ldiv0", &c, 20000)); CHECK_EQ_U(c, 0xC0000094u); }
TC_CASE(ldiv_filter_sees_code) { DWORD c = 0; CHECK(tc_spawn_self("ldiv0f", &c, 20000)); CHECK_EQ_U(c, 100); }

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (tc_strequ(c, "div0")) { div_t d = div(a, z); return d.quot; }
        if (tc_strequ(c, "ldiv0")) { ldiv_t d = ldiv(la, lz); return (int)d.quot; }
        if (tc_strequ(c, "ldiv0f")) { ldiv_t d; SetUnhandledExceptionFilter(filt); d = ldiv(la, lz); return (int)d.quot; }
        return 77;
    }
    TC_RUN(div_unhandled); TC_RUN(ldiv_unhandled); TC_RUN(ldiv_filter_sees_code);
    return tc_end();
}
