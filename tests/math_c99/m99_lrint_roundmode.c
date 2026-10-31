/* TC
 * id:    m99_lrint_roundmode
 * area:  math_c99
 * funcs: lrint llrint rint nearbyint
 * procs: _controlfp_s
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.12.9.4 (current rounding mode); MS docs _controlfp_s
 * note:  PROBE: lrint/llrint/rint/nearbyint honour the rounding mode set via _controlfp_s(_MCW_RC) (x64: MXCSR RC bits).
 *        Expected from C11 7.12.9 (current rounding mode); whether each CRT's implementation honours it is unverified.
 */
#include "tc.h"
#include <math.h>

TC_DECL_PROC(int, _controlfp_s, (unsigned int *, unsigned int, unsigned int));
#define RC_MASK 0x300u
#define RC_DOWN 0x100u
#define RC_UP   0x200u
#define RC_CHOP 0x300u

static void setrc(unsigned int rc) { unsigned int cur; if (tc_p__controlfp_s) tc_p__controlfp_s(&cur, rc, RC_MASK); }

TC_CASE(down) { volatile double a = 2.7, b = -2.2; if (!tc_p__controlfp_s) TC_SKIP("no _controlfp_s");
    setrc(RC_DOWN); CHECK_EQ_I(lrint(a), 2); CHECK_BITS_D(rint(a), 2.0); CHECK_EQ_I(lrint(b), -3); setrc(0); }
TC_CASE(up) { volatile double a = 2.2, b = -2.7; if (!tc_p__controlfp_s) TC_SKIP("no _controlfp_s");
    setrc(RC_UP); CHECK_EQ_I(lrint(a), 3); CHECK_BITS_D(rint(b), -2.0); setrc(0); }
TC_CASE(chop) { volatile double a = -2.9, b = 2.9; if (!tc_p__controlfp_s) TC_SKIP("no _controlfp_s");
    setrc(RC_CHOP); CHECK_EQ_I(llrint(a), -2); CHECK_BITS_D(nearbyint(b), 2.0); setrc(0); }
TC_CASE(restored) { volatile double a = 2.5; if (!tc_p__controlfp_s) TC_SKIP("no _controlfp_s");
    setrc(0); CHECK_EQ_I(lrint(a), 2); }

int main(void) { tc_begin(); TC_LOAD_PROC(_controlfp_s); TC_RUN(down); TC_RUN(up); TC_RUN(chop); TC_RUN(restored); return tc_end(); }
