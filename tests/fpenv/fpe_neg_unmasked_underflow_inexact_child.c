/* TC
 * id:    fpe_neg_unmasked_underflow_inexact_child
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _controlfp (unmasking an exception makes the FP operation raise an SEH exception); winnt.h STATUS_FLOAT_*
 * note:  PROBES that an unmasked underflow/inexact SSE exception is delivered as STATUS_FLOAT_UNDERFLOW/INEXACT_RESULT on x64. Fatal path in child: with the exception unmasked and no handler the process dies with the STATUS_FLOAT_*
 *        code as exit status. Parent also checks that the child did not reach its normal return (77).
 */
#include "tc.h"
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_max = 1.7976931348623157e308, v_two = 2.0, v_three = 3.0, v_tiny = 1e-200, v_inf;
static volatile double sink;

TC_CASE(inexact_traps)
{
    DWORD code = 0; int ran = tc_spawn_self("inexact", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0xC000008Fu);   /* STATUS_FLOAT_INEXACT_RESULT */
}
TC_CASE(underflow_traps)
{
    DWORD code = 0; int ran = tc_spawn_self("underflow", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0xC0000093u);   /* STATUS_FLOAT_UNDERFLOW */
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        _clearfp();
        if (c[0] == 'i') { _controlfp(0, _EM_INEXACT); sink = v_one / v_three; }
        else { _controlfp(0, _EM_UNDERFLOW); sink = v_tiny * v_tiny; }
        return 77;
    }
    TC_RUN(inexact_traps); TC_RUN(underflow_traps);
    return tc_end();
}
