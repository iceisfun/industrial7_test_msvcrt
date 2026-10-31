/* TC
 * id:    ucr_neg_strtod_overflow_underflow
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.22.1.3p10 (HUGE_VAL + ERANGE on overflow; underflow returns no more than the smallest normal, errno implementation-defined); MS docs strtod
 * note:  underflow errno is left to ucr_neg_strtod_underflow_errno_probe
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <math.h>

static const char *volatile v_s;
typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod

TC_CASE(overflow_pos)
{
    char *e; double d; v_s = "1e999"; errno = 0; d = strtod(v_s, &e);
    CHECK_BITS_D(d, HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v_s + 5);
}
TC_CASE(overflow_neg)
{
    char *e; double d; v_s = "-1e999"; errno = 0; d = strtod(v_s, &e);
    CHECK_BITS_D(d, -HUGE_VAL); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, v_s + 6);
}
TC_CASE(underflow_value)
{
    char *e; double d; v_s = "1e-999"; d = strtod(v_s, &e);
    CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, v_s + 6);
}
TC_CASE(no_digits)
{
    char *e = (char *)0x1; double d; v_s = "xyz"; d = strtod(v_s, &e);
    CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, v_s);
    v_s = "+"; d = strtod(v_s, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, v_s);
    v_s = ""; d = strtod(v_s, &e); CHECK_BITS_D(d, 0.0); CHECK_EQ_P(e, v_s);
}
int main(void)
{
    tc_begin();
    p_strtod = (strtod_t)tc_crt_proc("strtod");
    if (!p_strtod) { tc_write("TC FAIL strtod missing\n"); return 1; }
    TC_RUN(overflow_pos); TC_RUN(overflow_neg); TC_RUN(underflow_value); TC_RUN(no_digits);
    return tc_end();
}
