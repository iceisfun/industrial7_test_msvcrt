/* TC
 * id:    cv_neg_strtod_denormal_underflow_errno
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   C11 7.22.1.3 (underflow: errno ERANGE implementation-defined if result is not representable); MS docs strtod
 * note:  strtod("1e-400") underflows to 0 and sets ERANGE (MS docs strtod: errno ERANGE on out-of-range). Denormal-result
 *        errno behaviour is probed separately in cv_neg_strtod_denormal_errno_probe.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }

TC_CASE(underflow_to_zero) { double d; errno = 0; d = D("1e-400"); CHECK_BITS_D(d, 0.0); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(normal_no_errno) { errno = 0; D("1e-300"); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(underflow_to_zero); TC_RUN(normal_no_errno); return tc_end(); }
