/* TC
 * id:    pfv_exp_three_digits_all
 * area:  stdio_printf
 * funcs: sprintf _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.21.6.1; MS docs printf
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
static volatile double v1e100 = 1e100, v1em300 = 1e-300, vmax = 1.7976931348623157e308;
TC_CASE(e_1e100) { char b[64]; sprintf(b, "%e", v1e100); CHECK_STR(b, "1.000000e+100"); }
TC_CASE(e_1e_300) { char b[64]; sprintf(b, "%e", v1em300); CHECK_STR(b, "1.000000e-300"); }
TC_CASE(denormal_min) { char b[64]; volatile double d = D(1ULL); sprintf(b, "%e", d); CHECK_STR(b, "4.940656e-324"); }
TC_CASE(dbl_max) { char b[64]; sprintf(b, "%e", vmax); CHECK_STR(b, "1.797693e+308"); }
TC_CASE(wide_variant) { wchar_t w[64]; _snwprintf(w, 64, L"%e|%e", v1e100, v1em300); CHECK_WSTR(w, L"1.000000e+100|1.000000e-300"); }
int main(void) { tc_begin(); TC_RUN(e_1e100); TC_RUN(e_1e_300); TC_RUN(denormal_min); TC_RUN(dbl_max); TC_RUN(wide_variant); return tc_end(); }
