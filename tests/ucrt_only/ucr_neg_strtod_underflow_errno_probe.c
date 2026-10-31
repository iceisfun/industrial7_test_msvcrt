/* TC
 * id:    ucr_neg_strtod_underflow_errno_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs: strtod strtof
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.22.1.3p10 (underflow errno is implementation-defined)
 * note:  Probes whether ucrtbase sets ERANGE on total underflow (strtod 1e-999, strtof 1e-60) and whether the
 *        exactly representable denormal 5e-324 leaves errno untouched (guess: ERANGE is set only when the result is 0).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

static const char *volatile v_s;

TC_CASE(strtod_total_underflow) { char *e; v_s = "1e-999"; errno = 0; strtod(v_s, &e); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(strtof_total_underflow) { char *e; v_s = "1e-60"; errno = 0; strtof(v_s, &e); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(denormal_no_erange) { char *e; double d; v_s = "5e-324"; errno = 0; d = strtod(v_s, &e); CHECK_EQ_U(tc_dbits(d), 1); CHECK_EQ_I(errno, 0); }

int main(void) { tc_begin(); TC_RUN(strtod_total_underflow); TC_RUN(strtof_total_underflow); TC_RUN(denormal_no_erange); return tc_end(); }
