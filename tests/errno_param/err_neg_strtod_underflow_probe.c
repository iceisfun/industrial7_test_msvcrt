/* TC
 * id:    err_neg_strtod_underflow_probe
 * area:  errno_param
 * kind:  negative
 * funcs:
 * procs: strtod
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.1.3 (may set ERANGE on underflow); MS docs strtod
 * note:  PROBE: ERANGE on underflow-to-zero is standard-permitted but implementation defined; also errno state for a denormal result (assumed unchanged, 0). strtod resolved via procs (mingw links its own).
 */
#include "tc.h"
#include <stdlib.h>
typedef double (__cdecl *strtod_t)(const char *, char **);
static strtod_t p_strtod;
#define strtod p_strtod
#include <errno.h>

static const char *volatile v_tiny = "1e-999";
static const char *volatile v_den = "4.9406564584124654e-324";

TC_CASE(underflow_zero_value) { char *e; double r; r = strtod(v_tiny, &e); CHECK_BITS_D(r, 0.0); }
TC_CASE(underflow_erange) { char *e; errno = 0; strtod(v_tiny, &e); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(denormal_value) { char *e; double r; r = strtod(v_den, &e); CHECK_EQ_U(tc_dbits(r), 1); }
TC_CASE(denormal_errno_zero) { char *e; errno = 0; strtod(v_den, &e); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); p_strtod = (strtod_t)tc_crt_proc("strtod"); if (!p_strtod) { tc_write("TC FAIL " TC_TEST_ID "::__setup no strtod\n"); return 1; } TC_RUN(underflow_zero_value); TC_RUN(underflow_erange); TC_RUN(denormal_value); TC_RUN(denormal_errno_zero); return tc_end(); }
