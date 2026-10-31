/* TC
 * id:    cv_neg_strtof_range
 * area:  conv
 * funcs:
 * procs: strtof
 * crt:   >=120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.3p10 (HUGE_VALF on overflow, ERANGE); underflow split out to cv_neg_strtof_underflow_probe because ERANGE/result on underflow is implementation-defined; MS docs strtof
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <math.h>
#include <float.h>

static volatile float v_inf_src;
typedef float (__cdecl *strtof_t)(const char *, char **);
static strtof_t p_strtof;
static float F(const char *volatile s) { return p_strtof(s, NULL); }

TC_CASE(overflow_pos) { float f; errno = 0; f = F("1e39"); CHECK(f > FLT_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(overflow_neg) { float f; errno = 0; f = F("-1e39"); CHECK(f < -FLT_MAX); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(double_range_value) { float f; errno = 0; f = F("1e300"); CHECK(f > FLT_MAX); CHECK_EQ_U(tc_fbits(f), 0x7F800000u); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(endptr_after_overflow)
{
    const char *volatile s = "1e39z"; char *e = NULL; (void)p_strtof(s, &e); CHECK_EQ_P(e, s + 4);
}

int main(void) { tc_begin(); p_strtof = (strtof_t)tc_crt_proc("strtof"); if (!p_strtof) { tc_write("TC FAIL " TC_TEST_ID "::__strtof not exported\n"); return 1; } (void)v_inf_src; TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(double_range_value); TC_RUN(endptr_after_overflow); return tc_end(); }
