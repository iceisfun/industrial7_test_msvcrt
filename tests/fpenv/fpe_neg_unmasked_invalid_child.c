/* TC
 * id:    fpe_neg_unmasked_invalid_child
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _controlfp (unmasking an exception makes the FP operation raise an SEH exception); winnt.h STATUS_FLOAT_*
 * note:  Fatal path runs in a child process: with the exception unmasked and no handler the process dies with the STATUS_FLOAT_*
 *        code as exit status. Parent also checks that the child did not reach its normal return (77).
 */
#include "tc.h"
#include <float.h>

static volatile double v_zero = 0.0, v_one = 1.0, v_max = 1.7976931348623157e308, v_two = 2.0, v_three = 3.0, v_tiny = 1e-200, v_inf;
static volatile double sink;

TC_CASE(invalid_traps)
{
    DWORD code = 0; int ran = tc_spawn_self("invalid", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0xC0000090u);   /* STATUS_FLOAT_INVALID_OPERATION */
    CHECK(code != 77);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        _clearfp();
        v_inf = v_max * v_max;       /* overflow to +inf while OVERFLOW is still masked */
        _clearfp();
        _controlfp(0, _EM_INVALID);
        sink = v_inf - v_inf;
        return 77;
    }
    TC_RUN(invalid_traps);
    return tc_end();
}
