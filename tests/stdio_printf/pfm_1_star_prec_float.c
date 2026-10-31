/* TC
 * id:    pfm_1_star_prec_float
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p4; MS docs printf precision for e, f, g
 * note:  Exponent digits: ucrt prints at least 2, older CRTs 3. %F is only exercised on ucrt (msvcr120 %F support is covered by pff_f_cap_f_msvcr120_probe).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

/* default exponent digits: ucrt 2, older CRTs 3 (e+000) */
#if TC_AT_LEAST(140)
#define X0 ""
#else
#define X0 "0"
#endif

#define T(fmt, p, v, want) do { char b[64]; volatile int tp = (p); volatile double tv = (v); \
    int n = sprintf(b, fmt, tp, tv); CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(e) { T("%.*e", 2, 1.5, "1.50e+0" X0 "0"); T("%.*E", 2, 1.5, "1.50E+0" X0 "0"); T("%.*e", 0, 5.0, "5e+0" X0 "0"); }
TC_CASE(f) { T("%.*f", 3, 1.0, "1.000"); T("%.*f", 0, 7.0, "7"); T("%.*f", 1, -0.26, "-0.3"); }
TC_CASE(F) {
#if TC_AT_LEAST(140)
    T("%.*F", 3, 1.0, "1.000"); T("%.*F", 2, 3.5, "3.50");
#else
    TC_SKIP("%F only exercised on ucrt");
#endif
}
TC_CASE(g) { T("%.*g", 3, 3.14159, "3.14"); T("%.*g", 10, 0.5, "0.5"); T("%.*g", 3, 1234567.0, "1.23e+0" X0 "6"); }
TC_CASE(G) { T("%.*G", 3, 1e-5, "1E-0" X0 "5"); T("%.*G", 3, 3.14159, "3.14"); }

int main(void) { tc_begin(); TC_RUN(e); TC_RUN(f); TC_RUN(F); TC_RUN(g); TC_RUN(G); return tc_end(); }
