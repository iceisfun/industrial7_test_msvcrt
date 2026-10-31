/* TC
 * id:    scn_swscanf_float
 * area:  scanf
 * funcs: swscanf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.29.2.2; MS docs scanf

 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile const wchar_t *v;

TC_CASE(lf) { double d = 0; int r; v = L"3.5"; r = swscanf((const wchar_t *)v, L"%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 3.5); }
TC_CASE(f_exp) { float f = 0; int r; v = L"-1e2"; r = swscanf((const wchar_t *)v, L"%f", &f); CHECK_EQ_I(r, 1); CHECK_BITS_F(f, -100.0f); }
TC_CASE(leading_dot) { double d = 0; int r; v = L".25"; r = swscanf((const wchar_t *)v, L"%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 0.25); }
TC_CASE(width) { double d = 0; int r, k = -1; v = L"12345"; r = swscanf((const wchar_t *)v, L"%3lf%n", &d, &k); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, 123.0); CHECK_EQ_I(k, 3); }
TC_CASE(two) { double a = 0, b = 0; int r; v = L"1.5 +2.5E1"; r = swscanf((const wchar_t *)v, L"%lf %le", &a, &b); CHECK_EQ_I(r, 2); CHECK_BITS_D(a, 1.5); CHECK_BITS_D(b, 25.0); }
TC_CASE(neg_zero) { double d = 1; int r; v = L"-0.0"; r = swscanf((const wchar_t *)v, L"%lf", &d); CHECK_EQ_I(r, 1); CHECK_BITS_D(d, -0.0); }
int main(void) { tc_begin(); TC_RUN(lf); TC_RUN(f_exp); TC_RUN(leading_dot); TC_RUN(width); TC_RUN(two); TC_RUN(neg_zero); return tc_end(); }
