/* TC
 * id:    cv_neg_strtod_denormal_errno_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.22.1.3 (underflow: errno ERANGE implementation-defined if result is not representable); MS docs strtod
 * note:  OBSERVED (Win10 22H2 19045 ucrtbase, run twice): strtod("1e-310") and strtod("4.9e-324") return the correct non-zero denormal and
 *        errno stays 0 (ERANGE is NOT set for denormal results). C11 7.22.1.3p10 makes errno for denormal results implementation-defined, so
 *        this is a valid implementation choice, not a deviation. (ERANGE only for results rounding to zero/overflow, covered elsewhere.)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }

TC_CASE(denormal_1e_310_errno) { double d; errno = 0; d = D("1e-310"); CHECK(d > 0.0); CHECK_EQ_I(errno, 0); }
TC_CASE(denormal_min_errno) { double d; errno = 0; d = D("4.9e-324"); CHECK(d > 0.0); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(denormal_1e_310_errno); TC_RUN(denormal_min_errno);  return tc_end(); }
