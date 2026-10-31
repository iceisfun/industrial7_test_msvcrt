/* TC
 * id:    fpe_neg_controlfp_em_denormal_x64_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp _clearfp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _controlfp (_EM_DENORMAL = 0x00080000, part of _MCW_EM)
 * note:  PROBE: unmasking _EM_DENORMAL on x64. Float.h value is 0x80000 (not 0x2). Whether the readback clears the bit, and whether a denormal operand traps (0xC000008D) or not (77), is unknown; both outcomes are accepted for the child, the readback is logged only.
 */
#include "tc.h"
#include <float.h>
#define ALLRC (_MCW_EM | _MCW_RC | _MCW_DN)
static void logx(const char *what, unsigned long long v) { char b[32]; tc_hex(v, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write("="); tc_write(b); tc_write("\n"); }

TC_CASE(readback)
{
    unsigned orig = _controlfp(0, 0), w;
    _controlfp(0, _EM_DENORMAL);
    w = _controlfp(0, 0);
    logx("after_unmask", w);
    _controlfp(_EM_DENORMAL, _EM_DENORMAL);
    logx("after_remask", _controlfp(0, 0));   /* logged only */
    _controlfp(orig, _MCW_EM); _clearfp();
}
TC_CASE(denormal_operand_child)
{
    DWORD c = 0; int ran = tc_spawn_self("denorm", &c, 20000);
    CHECK(ran); logx("child", c);
    CHECK(c == 77 || c == 0xC000008Du);
}
int main(void)
{
    tc_begin();
    if (tc_child_name()) {
        static volatile double d = 4.9406564584124654e-324, one = 1.0, sink;
        _clearfp(); _controlfp(0, _EM_DENORMAL);
        sink = d * one;
        return 77;
    }
    TC_RUN(readback); TC_RUN(denormal_operand_child);
    return tc_end();
}
