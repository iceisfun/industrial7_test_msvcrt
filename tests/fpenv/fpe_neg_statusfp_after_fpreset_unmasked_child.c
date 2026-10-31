/* TC
 * id:    fpe_neg_statusfp_after_fpreset_unmasked_child
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * procs: _fpreset
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _fpreset (resets the floating-point package to its default state)
 * note:  PROBE: _fpreset re-masks previously unmasked exceptions, so a later 1.0/0.0 does not trap. (On x64 old CRTs _fpreset may only touch x87 state; hence probe.) The unmasked-without-reset control is covered by fpe_neg_unmasked_divzero_child.
 */
#include "tc.h"
#include <float.h>
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_DECL_PROC(void, _fpreset, (void));
TC_CASE(reset_remasks)
{
    DWORD c = 0; int ran = tc_spawn_self("reset", &c, 20000);
    CHECK(ran); logx("child", c);
    CHECK_EQ_U(c, 0);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        static volatile double z = 0.0, one = 1.0, sink;
        _clearfp();
        _controlfp(0, _EM_ZERODIVIDE);
        if (!TC_LOAD_PROC(_fpreset)) return 4;
        tc_p__fpreset();
        sink = one / z;
        return (_controlfp(0, 0) & 0x1f) == 0x1f ? 0 : 5;
    }
    TC_RUN(reset_remasks);
    return tc_end();
}
