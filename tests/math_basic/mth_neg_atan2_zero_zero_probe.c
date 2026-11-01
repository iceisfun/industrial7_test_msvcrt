/* TC
 * id:    mth_neg_atan2_zero_zero_probe
 * area:  math_basic
 * funcs:
 * procs: atan2
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs atan2
 * note:  F: TC_UCRT is never defined by tools/tc.py (it passes -D_UCRT), so the EDOM branch ran on ucrtbase; now #if defined(_UCRT). Observed errno=0 on Win10 19045 ucrtbase (raw of the branch build calling the DLL atan2 via GetProcAddress, both reference runs: TC INFO atan2(0,0) errno=0; the baseline INFO line came from the static libmingwex atan2 and is not evidence). Was: Probe: atan2(0,0) returns +0 (documented); errno is EDOM on pre-UCRT per old docs, unknown on ucrt (only recorded there). Observed on an FMA3/AVX2 CPU (ucrtbase dispatches FMA3 paths); non-FMA path unverified.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>
#include <limits.h>
#include <float.h>
/* F: call the DLL export, never the libmingwex static copy (tc_crt_proc = GetProcAddress) */
TC_DECL_PROC(double, atan2, (double, double));
#undef atan2
#define atan2 tc_p_atan2

static volatile double v_zero = 0.0, v_one = 1.0;
static double pinf(void) { return v_one / v_zero; }
static void note_errno(const char *what, int e) { char b[24]; tc_i64(e, b); tc_write("TC INFO " TC_TEST_ID " "); tc_write(what); tc_write(" errno="); tc_write(b); tc_write("\n"); }

TC_CASE(value) { volatile double y = 0.0, x = 0.0; CHECK_BITS_D(atan2(y, x), 0.0); }
TC_CASE(errno_recorded) { volatile double y = 0.0, x = 0.0; double r; errno = 0; r = atan2(y, x); (void)r; note_errno("atan2(0,0)", errno);
#if defined(_UCRT)
    CHECK_EQ_I(errno, 0);   /* observed Win10 19045 ucrtbase: atan2(0,0) leaves errno 0 */
#else
    CHECK_EQ_I(errno, EDOM);
#endif
}

int main(void) { tc_begin(); TC_LOAD_PROC(atan2); if (!tc_p_atan2) { tc_write("TC FAIL " TC_TEST_ID "::proc missing\n"); return 1; } TC_RUN(value); TC_RUN(errno_recorded); return tc_end(); }
