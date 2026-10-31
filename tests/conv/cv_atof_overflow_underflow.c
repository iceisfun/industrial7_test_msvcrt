/* TC
 * id:    cv_atof_overflow_underflow
 * area:  conv
 * funcs: atof
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3p10 (HUGE_VAL on overflow, 0 on underflow); MS docs atof
 * note:  errno/ERANGE, limits and signed-zero underflow moved to cv_atof_erange_probe, cv_atof_limits_ucrt, cv_atof_limits_legacy_probe
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <float.h>

static double F(const char *volatile s) { return atof(s); }

TC_CASE(overflow) { double d = F("1e309"); CHECK(d > DBL_MAX); d = F("-1e309"); CHECK(d < -DBL_MAX); }
TC_CASE(underflow) { CHECK_BITS_D(F("1e-400"), 0.0); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(underflow); return tc_end(); }
