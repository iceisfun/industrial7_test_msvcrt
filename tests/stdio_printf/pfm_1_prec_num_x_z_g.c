/* TC
 * id:    pfm_1_prec_num_x_z_g
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1p8; MS docs printf precision
 * note:  name kept from spec; %Z moved to pfm_1_prec_z_probe. Exponent digits: ucrt 2, older 3. %F only on ucrt.
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

#define TU(fmt, v, want) do { char b[64]; volatile unsigned tv = (v); int n = sprintf(b, fmt, tv); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)
#define TD(fmt, v, want) do { char b[64]; volatile double tv = (v); int n = sprintf(b, fmt, tv); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(X) { TU("%.8X", 0xBEEF, "0000BEEF"); TU("%#.8X", 0xBEEF, "0X0000BEEF"); TU("%.2X", 0xBEEF, "BEEF"); TU("%12.8X", 0xBEEF, "    0000BEEF"); }
TC_CASE(G) { TD("%.4G", 123456.0, "1.235E+0" X0 "5"); TD("%.4G", 0.00001234, "1.234E-0" X0 "5"); TD("%.4G", 1.5, "1.5"); TD("%#.4G", 1.5, "1.500"); }
TC_CASE(F) {
#if TC_AT_LEAST(140)
    TD("%.2F", 3.14159, "3.14"); TD("%10.2F", 3.14159, "      3.14");
#else
    TC_SKIP("%F only exercised on ucrt");
#endif
}

int main(void) { tc_begin(); TC_RUN(X); TC_RUN(G); TC_RUN(F); return tc_end(); }
