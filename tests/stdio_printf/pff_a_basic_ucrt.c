/* TC
 * id:    pff_a_basic_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.1 a/A; MS docs printf type field a/A
 * note:  explicit precision avoids default-precision differences
 */
#include "tc.h"
#include <stdio.h>

static volatile double one = 1.0, onehalf = 1.5, v255 = 255.0, mhalf = -0.5, tenth = 0.1;

TC_CASE(p3) { char b[64]; sprintf(b, "%.3a", one); CHECK_STR(b, "0x1.000p+0"); }
TC_CASE(p1) { char b[64]; sprintf(b, "%.1a", onehalf); CHECK_STR(b, "0x1.8p+0"); }
TC_CASE(upper) { char b[64]; sprintf(b, "%.2A", v255); CHECK_STR(b, "0X1.FEP+7"); }
TC_CASE(p0) { char b[64]; sprintf(b, "%.0a", one); CHECK_STR(b, "0x1p+0"); }
TC_CASE(p0_alt) { char b[64]; sprintf(b, "%#.0a", one); CHECK_STR(b, "0x1.p+0"); }
TC_CASE(negative) { char b[64]; sprintf(b, "%.4a", mhalf); CHECK_STR(b, "-0x1.0000p-1"); }
TC_CASE(tenth13) { char b[64]; sprintf(b, "%.13a", tenth); CHECK_STR(b, "0x1.999999999999ap-4"); }

int main(void) { tc_begin(); TC_RUN(p3); TC_RUN(p1); TC_RUN(upper); TC_RUN(p0); TC_RUN(p0_alt); TC_RUN(negative); TC_RUN(tenth13); return tc_end(); }
