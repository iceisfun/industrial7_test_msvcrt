/* TC
 * id:    mf_neg_expf_underflow_errno_probe
 * area:  math_float
 * funcs:
 * procs: expf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.12.1 (underflow may set ERANGE); MS docs exp
 * note:  PROBE: whether expf underflow to 0 (x=-1000) sets errno=ERANGE on ucrt (most likely yes); expf(-100) denormal result errno is only logged as a skip-free value, asserted to be 0 or ERANGE.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

TC_DECL_PROC(float, expf, (float));
#undef expf
#define expf tc_p_expf
#include <float.h>

static volatile float v_zero = 0.0f, v_one = 1.0f;
static float F_INF(void) { return v_one / v_zero; }
static float F_NAN(void) { return v_zero / v_zero; }

TC_CASE(underflow_to_zero) { volatile float x = -1000.0f; errno = 0; (void)expf(x); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(denormal_result) {
    volatile float x = -100.0f; float r; char b[24];
    errno = 0; r = expf(x);
    CHECK(r >= 0.0f && r <= FLT_MIN);
    tc_i64(errno, b); tc_write("TC INFO errno after expf(-100)="); tc_write(b); tc_write("\n");
    CHECK(errno == 0 || errno == ERANGE);
}

int main(void) { tc_begin(); TC_LOAD_PROC(expf); if (!(tc_p_expf)) { tc_write("TC FAIL "TC_TEST_ID"::proc missing\n"); return 1; } TC_RUN(underflow_to_zero); TC_RUN(denormal_result); return tc_end(); }
