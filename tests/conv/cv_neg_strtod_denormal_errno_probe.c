/* TC
 * id:    cv_neg_strtod_denormal_errno_probe
 * area:  conv
 * funcs:
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.3 (underflow: errno ERANGE implementation-defined if result is not representable); MS docs strtod
 * note:  Probes only: whether UCRT strtod sets ERANGE for denormal (non-zero, subnormal) results; guessed ERANGE. C11 leaves this
 *        implementation-defined.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }

TC_CASE(denormal_1e_310_errno) { double d; errno = 0; d = D("1e-310"); CHECK(d > 0.0); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(denormal_min_errno) { double d; errno = 0; d = D("4.9e-324"); CHECK(d > 0.0); CHECK_EQ_I(errno, ERANGE); }

int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(denormal_1e_310_errno); TC_RUN(denormal_min_errno);  return tc_end(); }
