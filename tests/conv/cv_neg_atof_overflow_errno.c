/* TC
 * id:    cv_neg_atof_overflow_errno
 * area:  conv
 * funcs:
 * procs: strtod

 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs strtod (ERANGE, HUGE_VAL, 0 on underflow); C11 7.22.1.3p10
 * note:  atof errno behaviour is probed separately in cv_atof_erange_probe.

 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <math.h>

typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t strtod_p;
#define strtod strtod_p

static const char *volatile s_big = "1e999", *volatile s_nbig = "-1e999", *volatile s_small = "1e-999", *volatile s_ok = "1.5e3x";

TC_CASE(overflow_pos) { char *e; double d; errno = 0; d = strtod(s_big, &e); CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s_big + 5); }
TC_CASE(overflow_neg) { char *e; double d; errno = 0; d = strtod(s_nbig, &e); CHECK_BITS_D(d, -HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s_nbig + 6); }
TC_CASE(underflow)    { char *e; double d; errno = 0; d = strtod(s_small, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, s_small + 6); }
TC_CASE(in_range_errno_kept) { char *e; double d; errno = 0; d = strtod(s_ok, &e); CHECK_BITS_D(d, 1500.0); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, s_ok + 5); }

int main(void) { tc_begin(); strtod_p = (strtod_t)tc_crt_proc("strtod"); if (!strtod_p) { tc_write("TC FAIL " TC_TEST_ID "::__missing export\n"); return 2; } TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow); TC_RUN(in_range_errno_kept); return tc_end(); }
