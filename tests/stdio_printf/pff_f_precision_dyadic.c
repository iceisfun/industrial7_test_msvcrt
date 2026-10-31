/* TC
 * id:    pff_f_precision_dyadic
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 f; exact decimal expansion of dyadic values
 * note:  Values have short exact expansions (<=14 significant digits) so legacy and UCRT agree.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

/* sprintf one double with fmt, compare string and return value (== strlen of expected) */
#define TF(fmt, val, want) do { char b[1100]; volatile double tc_v = (val); int n = sprintf(b, fmt, tc_v); \
    CHECK_STR(b, want); CHECK_EQ_I(n, (int)(sizeof(want) - 1)); } while (0)

TC_CASE(half100)
{
    char b[300], w[300]; volatile double v = 0.5; int n = sprintf(b, "%.100f", v);
    memset(w, '0', sizeof w); w[0] = '0'; w[1] = '.'; w[2] = '5'; w[102] = 0;
    CHECK_STR(b, w); CHECK_EQ_I(n, 102); CHECK_EQ_I((int)strlen(b), 102);
}
TC_CASE(pow2) { TF("%.30f", 9.5367431640625e-07, "0.000000953674316406250000000000"); }
TC_CASE(sixteenth) { TF("%.20f", 0.0625, "0.06250000000000000000"); }
TC_CASE(k) { TF("%.50f", 1024.0, "1024.00000000000000000000000000000000000000000000000000"); }

int main(void) { tc_begin(); TC_RUN(half100); TC_RUN(pow2); TC_RUN(sixteenth); TC_RUN(k); return tc_end(); }
