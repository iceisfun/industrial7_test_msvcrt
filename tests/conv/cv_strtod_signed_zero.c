/* TC
 * id:    cv_strtod_signed_zero
 * area:  conv
 * funcs: atof
 * procs: strtod
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.22.1.3 (sign is part of the subject sequence), F.3 (signed zero)
 */
#include "tc.h"
#include <stdlib.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

static double D(const char *volatile s) { return strtod(s, NULL); }
static double A(const char *volatile s) { return atof(s); }

TC_CASE(neg_zero) { CHECK_BITS_D(D("-0"), -0.0); CHECK_BITS_D(A("-0"), -0.0); }
TC_CASE(pos_zero) { CHECK_BITS_D(D("+0"), 0.0); CHECK_BITS_D(A("+0"), 0.0); }
TC_CASE(neg_zero_exp) { CHECK_BITS_D(D("-0.0e5"), -0.0); CHECK_BITS_D(A("-0.0e5"), -0.0); }
TC_CASE(neg_zero_frac) { CHECK_BITS_D(D("-0.000"), -0.0); }

int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod");  TC_RUN(neg_zero); TC_RUN(pos_zero); TC_RUN(neg_zero_exp); TC_RUN(neg_zero_frac); return tc_end(); }
