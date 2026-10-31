/* TC
 * id:    pfm_1_bare_dot_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4; MS docs printf precision
 * note:  non-tie inputs except exact ties where both rounding rules agree (1.5 -> 2, 3.5 -> 4). Exponent digits: ucrt 2, older 3. %F only on ucrt.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

#if TC_AT_LEAST(140)
#define X0 ""
#else
#define X0 "0"
#endif

#define T(fmt, v, want) do { char b[64]; volatile double tv = (v); int n = sprintf(b, fmt, tv); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(f) { T("%.f", 3.5, "4"); T("%.f", 7.2, "7"); T("%#.f", 7.2, "7."); }
TC_CASE(e) { T("%.e", 1.5, "2e+0" X0 "0"); T("%.E", 1.5, "2E+0" X0 "0"); T("%#.e", 1.0, "1.e+0" X0 "0"); }
TC_CASE(g) { T("%.g", 1.5, "2"); T("%.G", 1e-5, "1E-0" X0 "5"); T("%.g", 0.5, "0.5"); }
TC_CASE(F) {
#if TC_AT_LEAST(140)
    T("%.F", 2.0, "2"); T("%.F", 2.7, "3");
#else
    TC_SKIP("%F only exercised on ucrt");
#endif
}

int main(void) { tc_begin(); TC_RUN(f); TC_RUN(e); TC_RUN(g); TC_RUN(F); return tc_end(); }
