/* TC
 * id:    pfm_1_prec_zero_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p8 (E, F, G with precision 0); MS docs printf precision
 * note:  Exponent digits: ucrt 2, older 3. 15.0 and 1.5 are exact ties where both rounding rules give 2. %F only on ucrt.
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

TC_CASE(E) { T("%.0E", 15.0, "2E+0" X0 "1"); T("%.0E", 9.6, "1E+0" X0 "1"); }
TC_CASE(F) {
#if TC_AT_LEAST(140)
    T("%.0F", 1.5, "2"); T("%.0F", 0.2, "0");
#else
    TC_SKIP("%F only exercised on ucrt");
#endif
}
TC_CASE(G) { T("%.0G", 0.5, "0.5"); T("%.0G", 1234.0, "1E+0" X0 "3"); T("%.0G", 7.0, "7"); }

int main(void) { tc_begin(); TC_RUN(E); TC_RUN(F); TC_RUN(G); return tc_end(); }
