/* TC
 * id:    pff_f_exact_digits_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs printf precision (UCRT, VS2015+)
 * note:  Expected strings are the exact decimal expansion of the stored double, correctly rounded (computed offline with arbitrary-precision arithmetic).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

static volatile double v_one = 1.0, v_three = 3.0, v_two = 2.0;
TC_CASE(tenth20) { TF("%.20f", 0.1, "0.10000000000000000555"); }
TC_CASE(tenth30) { TF("%.30f", 0.1, "0.100000000000000005551115123126"); }
TC_CASE(third25) { TF("%.25f", v_one / v_three, "0.3333333333333333148296163"); }
TC_CASE(tiny) { TF("%.30f", 1e-10, "0.000000000100000000000000003643"); }
TC_CASE(twothirds) { TF("%.20f", v_two / v_three, "0.66666666666666662966"); }

int main(void) { tc_begin(); TC_RUN(tenth20); TC_RUN(tenth30); TC_RUN(third25); TC_RUN(tiny); TC_RUN(twothirds); return tc_end(); }
