/* TC
 * id:    cv_strtod_underflow_sign_probe
 * area:  conv
 * funcs: atof
 * procs: strtod
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   IEC 60559 (underflow keeps sign); C11 7.22.1.3
 * note:  Probes: strtod("-1e-400") underflows to -0.0 (sign bit preserved), also for atof.
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }
TC_CASE(strtod_neg_underflow) { CHECK_BITS_D(D("-1e-400"), -0.0); }
TC_CASE(atof_neg_underflow) { volatile const char *s = "-1e-400"; CHECK_BITS_D(atof((const char *)s), -0.0); }
int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); TC_RUN(strtod_neg_underflow); TC_RUN(atof_neg_underflow); return tc_end(); }
