/* TC
 * id:    cv_atof_erange_probe
 * area:  conv
 * funcs: atof
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs atof (ERANGE on out-of-range); C11 7.22.1.3p10 permits errno ERANGE for strtod family
 * note:  PROBE: atof sets errno=ERANGE on overflow and on underflow. Legacy (pre-UCRT) atof may not touch errno at all.
 *        Overflow and underflow are separate cases.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <float.h>

static double F(const char *volatile s) { return atof(s); }

TC_CASE(overflow) { double d; errno = 0; d = F("1e309"); CHECK(d > DBL_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(underflow) { errno = 0; (void)F("1e-400"); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(underflow); return tc_end(); }
