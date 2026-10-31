/* TC
 * id:    mf_nextaft_scalb_logb_f_probe
 * area:  math_float
 * funcs:
 * procs: _nextafterf _logbf _scalbf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _nextafter, _logb, _scalb (float.h; float variants _nextafterf/_logbf/_scalbf, x64)
 */
#include "tc.h"
TC_DECL_PROC(float, _nextafterf, (float, float)); TC_DECL_PROC(float, _logbf, (float)); TC_DECL_PROC(float, _scalbf, (float, long));
static volatile float v1 = 1.0f, v2 = 2.0f, v0 = 0.0f, v8 = 8.0f, vh = 0.5f;
static volatile long l4 = 4, lm1 = -1;

TC_CASE(nextafter_up) { if (!tc_p__nextafterf) TC_SKIP("absent"); CHECK_EQ_U(tc_fbits(tc_p__nextafterf(v1, v2)), 0x3F800001u); }
TC_CASE(nextafter_down) { if (!tc_p__nextafterf) TC_SKIP("absent"); CHECK_EQ_U(tc_fbits(tc_p__nextafterf(v1, v0)), 0x3F7FFFFFu); }
TC_CASE(logb) { if (!tc_p__logbf) TC_SKIP("absent"); CHECK_BITS_F(tc_p__logbf(v8), 3.0f); CHECK_BITS_F(tc_p__logbf(vh), -1.0f); }
TC_CASE(scalb) { if (!tc_p__scalbf) TC_SKIP("absent"); CHECK_BITS_F(tc_p__scalbf(v1, l4), 16.0f); CHECK_BITS_F(tc_p__scalbf(v1, lm1), 0.5f); }
int main(void)
{
    tc_begin();
    TC_LOAD_PROC(_nextafterf); TC_LOAD_PROC(_logbf); TC_LOAD_PROC(_scalbf);
    TC_RUN(nextafter_up); TC_RUN(nextafter_down); TC_RUN(logb); TC_RUN(scalb);
    return tc_end();
}
