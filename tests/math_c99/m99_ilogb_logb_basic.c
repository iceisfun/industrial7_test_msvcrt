/* TC
 * id:    m99_ilogb_logb_basic
 * area:  math_c99
 * funcs: ilogb ilogbf logb logbf _logb
 * procs: 
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.12.6.5, 7.12.6.11; MS docs ilogb, logb, _logb
 * note:  subnormal inputs are normalised (ilogb(min subnormal) = -1074)
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
static volatile double v_a; static volatile float v_f;
TC_CASE(ilogb_norm) { v_a = 8; CHECK_EQ_I(ilogb(v_a), 3); v_a = 1; CHECK_EQ_I(ilogb(v_a), 0); v_a = 0.5; CHECK_EQ_I(ilogb(v_a), -1); v_a = -1024; CHECK_EQ_I(ilogb(v_a), 10); }
TC_CASE(ilogb_sub) { union { unsigned long long u; double d; } m; m.u = 0x0010000000000000ULL; v_a = m.d; CHECK_EQ_I(ilogb(v_a), -1022);
    m.u = 1; v_a = m.d; CHECK_EQ_I(ilogb(v_a), -1074); }
TC_CASE(logb_v) { v_a = 8; CHECK_BITS_D(logb(v_a), 3.0); v_a = 0.75; CHECK_BITS_D(logb(v_a), -1.0); }
TC_CASE(logbf_v) { v_f = 16; CHECK_BITS_F(logbf(v_f), 4.0f); v_f = 5; CHECK_EQ_I(ilogbf(v_f), 2); }
TC_CASE(legacy) { v_a = 8; CHECK_BITS_D(_logb(v_a), 3.0); }
int main(void) { tc_begin(); TC_RUN(ilogb_norm); TC_RUN(ilogb_sub); TC_RUN(logb_v); TC_RUN(logbf_v); TC_RUN(legacy); return tc_end(); }
