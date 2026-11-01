/* TC
 * id:    gcv_neg_imaxdiv_zero_child
 * area:  conv
 * funcs: imaxdiv
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C99 7.8.2.2 (undefined when result not representable)
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): imaxdiv(1,0) terminates the child with 0xC0000094 (integer divide by zero);
 *        imaxdiv(INTMAX_MIN,-1) (and lldiv) raises STATUS_INTEGER_OVERFLOW 0xC0000095 (hardware idiv overflow) and the child ends with that code.
 *        The earlier "hang" was a HARNESS artifact (class F): mingw-w64's _gnu_exception_handler (SetUnhandledExceptionFilter in crtexe) returns
 *        CONTINUE_EXECUTION for 0xC0000095 so the idiv re-ran forever. The child now calls tc_seh_default() first. Undefined in C (C11 6.5.5p6).
 */
#include "tc.h"
#include <inttypes.h>
#include <stdint.h>

static volatile intmax_t n, d;

static int child_main(const char *name)
{
    imaxdiv_t r;
    tc_seh_default();
    if (tc_strequ(name, "div0")) { n = 1; d = 0; }
    else if (tc_strequ(name, "ovf")) { n = INTMAX_MIN; d = -1; }
    else return 13;
    r = imaxdiv(n, d);
    return (int)(r.quot & 0) + 77;
}
TC_CASE(div0) { DWORD code = 0; int ok = tc_spawn_self("div0", &code, 5000); CHECK(ok); CHECK_EQ_U(code, 0xC0000094u); }
TC_CASE(ovf) { DWORD code = 0; int ok = tc_spawn_self("ovf", &code, 5000); CHECK(ok); CHECK_EQ_U(code, 0xC0000095u); }

int main(void)
{
    tc_begin();
    if (tc_child_name()) return child_main(tc_child_name());
    TC_RUN(div0); TC_RUN(ovf);
    return tc_end();
}
