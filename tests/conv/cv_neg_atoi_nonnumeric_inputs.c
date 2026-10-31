/* TC
 * id:    cv_neg_atoi_nonnumeric_inputs
 * area:  conv
 * funcs: atoi atol _atoi64 atof
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.2 (no conversion -> zero value); MS docs atoi, atof
 * note:  Inputs with no convertible digits must return 0 (no crash, no invalid-parameter). errno is probed separately.
 */
#include "tc.h"
#include <stdlib.h>

static const char *volatile in;
#define ALL_ZERO(s) do { in = (s); CHECK_EQ_I(atoi(in), 0); CHECK_EQ_I(atol(in), 0); CHECK_EQ_I(_atoi64(in), 0); CHECK_BITS_D(atof(in), 0.0); } while (0)

TC_CASE(alpha) { ALL_ZERO("abc"); ALL_ZERO("x12"); }
TC_CASE(empty) { ALL_ZERO(""); }
TC_CASE(blank) { ALL_ZERO("   "); ALL_ZERO(" \t\r\n"); }
TC_CASE(lone_sign) { ALL_ZERO("-"); ALL_ZERO("+"); ALL_ZERO("- 5"); ALL_ZERO("+-5"); }
TC_CASE(atof_empty) { in = ""; CHECK_BITS_D(atof(in), 0.0); in = "."; CHECK_BITS_D(atof(in), 0.0); }

int main(void) { tc_begin(); TC_RUN(alpha); TC_RUN(empty); TC_RUN(blank); TC_RUN(lone_sign); TC_RUN(atof_empty); return tc_end(); }
