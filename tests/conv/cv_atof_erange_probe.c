/* TC
 * id:    cv_atof_erange_probe
 * area:  conv
 * funcs: atof
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs atof (ERANGE on out-of-range); C11 7.22.1.3p10 permits errno ERANGE for strtod family
 * note:  OBSERVED on Windows 10 22H2 (19045) ucrtbase: atof("1e309") returns +inf and atof("1e-400") returns 0 but errno stays 0
 *        (ERANGE is NOT set), although the MS atof page says errno is set to ERANGE in all out-of-range cases. Deterministic (run twice).
 *        The original probe guessed ERANGE; real Windows disagrees, so the expectation is now errno unchanged. Overflow and underflow are separate cases.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <float.h>

#if TC_AT_LEAST(140)
#define EXP_ERRNO 0        /* observed: ucrtbase atof leaves errno alone */
#else
#define EXP_ERRNO ERANGE   /* unverified guess for pre-UCRT CRTs */
#endif
static double F(const char *volatile s) { return atof(s); }

TC_CASE(overflow) { double d; errno = 0; d = F("1e309"); CHECK(d > DBL_MAX); CHECK_EQ_I(errno, EXP_ERRNO); }
TC_CASE(underflow) { errno = 0; (void)F("1e-400"); CHECK_EQ_I(errno, EXP_ERRNO); }

int main(void) { tc_begin(); TC_RUN(overflow); TC_RUN(underflow); return tc_end(); }
