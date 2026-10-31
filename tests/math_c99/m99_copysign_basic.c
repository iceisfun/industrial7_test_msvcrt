/* TC
 * id:    m99_copysign_basic
 * area:  math_c99
 * funcs: _copysign _copysignf _chgsign _chgsignf
 * procs: copysign copysignf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.12.11.1, F.10.8.1; MS docs copysign, _copysign, _chgsign
 * note:  _chgsign negates including zeros.
 */
#include "tc.h"
#include <math.h>
TC_DECL_PROC(double, copysign, (double, double));
TC_DECL_PROC(float, copysignf, (float, float));
static volatile double v_zero = 0.0, v_one = 1.0;
static volatile float f_zero = 0.0f, f_one = 1.0f;
TC_CASE(finite) { volatile double a = 3.0, b = -3.0, n = -1.0, p = 1.0, pz = 0.0, nz = -0.0;
    CHECK_BITS_D(tc_p_copysign(a, n), -3.0); CHECK_BITS_D(tc_p_copysign(b, p), 3.0); CHECK_BITS_D(tc_p_copysign(a, pz), 3.0); CHECK_BITS_D(tc_p_copysign(a, nz), -3.0); }
TC_CASE(zeros) { volatile double pz = 0.0, nz = -0.0, n = -1.0, p = 1.0;
    CHECK_BITS_D(tc_p_copysign(pz, n), -0.0); CHECK_BITS_D(tc_p_copysign(nz, p), 0.0); }
TC_CASE(inf) { double i = v_one / v_zero; volatile double n = -1.0; CHECK_BITS_D(tc_p_copysign(i, n), -i); CHECK_BITS_D(tc_p_copysign(-i, 1.0 * v_one), i); }
TC_CASE(float_variants) { volatile float a = 2.0f, nz = -0.0f, n = -4.0f, p = 4.0f; CHECK_BITS_F(tc_p_copysignf(a, nz), -2.0f); CHECK_BITS_F(tc_p_copysignf(-a, p), 2.0f); CHECK_BITS_F(tc_p_copysignf(a, n), -2.0f); }
TC_CASE(underscore_copysign) { volatile double a = 3.0, n = -1.0; volatile float fa = 2.0f, fn = -1.0f;
    CHECK_BITS_D(_copysign(a, n), -3.0); CHECK_BITS_D(_copysign(-a, 1.0 * v_one), 3.0); CHECK_BITS_F(_copysignf(fa, fn), -2.0f); CHECK_BITS_F(_copysignf(-fa, f_one), 2.0f); }
TC_CASE(chgsign) { volatile double a = 1.0, nz = -0.0, pz = 0.0; volatile float fa = 1.0f, fnz = -0.0f;
    CHECK_BITS_D(_chgsign(a), -1.0); CHECK_BITS_D(_chgsign(-a), 1.0); CHECK_BITS_D(_chgsign(nz), 0.0); CHECK_BITS_D(_chgsign(pz), -0.0);
    CHECK_BITS_F(_chgsignf(fa), -1.0f); CHECK_BITS_F(_chgsignf(fnz), 0.0f); (void)f_zero; }
int main(void) { tc_begin(); TC_LOAD_PROC(copysign); TC_LOAD_PROC(copysignf); if (!tc_p_copysign || !tc_p_copysignf) { tc_write("TC FAIL copysign not exported\n"); return 1; } TC_RUN(finite); TC_RUN(zeros); TC_RUN(inf); TC_RUN(float_variants); TC_RUN(underscore_copysign); TC_RUN(chgsign); return tc_end(); }
