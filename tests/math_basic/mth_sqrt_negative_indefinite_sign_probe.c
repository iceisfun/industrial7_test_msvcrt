/* TC
 * id:    mth_sqrt_negative_indefinite_sign_probe
 * area:  math_basic
 * funcs: sqrt
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 F.9.4.5 (NaN sign unspecified); MS docs sqrt
 * note:  Probing the NaN sign/payload of sqrt(-1) (printed as -1.#IND): expected 0xFFF8000000000000 like hardware sqrtsd.
 */
#include "tc.h"
#include <math.h>
#include <errno.h>

static volatile double v_zero = 0.0, v_one = 1.0;
static double tc_inf(void) { return v_one / v_zero; }
/* result is NaN and errno == err */
#define NAN_ERR(expr, err) do { volatile double tc_r; errno = 0; tc_r = (expr); CHECK_IS_NAN(tc_r); CHECK_EQ_I(errno, err); } while (0)

TC_CASE(sqrt_m1_bits)
{
    volatile double x = -1.0;
    errno = 0;
    CHECK_EQ_U(tc_dbits(sqrt(x)), 0xFFF8000000000000ULL);
}

int main(void)
{
    tc_begin();
    TC_RUN(sqrt_m1_bits);
    return tc_end();
}
