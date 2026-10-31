/* TC
 * id:    pfv_longdouble_lf
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs printf size prefixes / Data type storage: long double is the same as double
 * note:  Passes a double to %Lf etc.; the following %d proves exactly 8 bytes were consumed. Compiler format warnings are disabled.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

static volatile double d;
TC_CASE(Lf) { char b[64]; d = 1.5; sprintf(b, "%Lf", d); CHECK_STR(b, "1.500000"); }
TC_CASE(Le_exp) { char b[64]; d = 12345.0; sprintf(b, "%.2Le", d); CHECK_STR(b, TC_CRT_VER >= 140 ? "1.23e+04" : "1.23e+004"); }
TC_CASE(Lg) { char b[64]; d = 0.25; sprintf(b, "%Lg", d); CHECK_STR(b, "0.25"); }
TC_CASE(lf_same) { char b[64]; d = 2.75; sprintf(b, "%lf", d); CHECK_STR(b, "2.750000"); }
TC_CASE(mixed_following_arg) { char b[64]; d = 3.5; sprintf(b, "%.1Lf|%d|%.2Lg|%d", d, 77, d, 88); CHECK_STR(b, "3.5|77|3.5|88"); }

int main(void) { tc_begin(); TC_RUN(Lf); TC_RUN(Le_exp); TC_RUN(Lg); TC_RUN(lf_same); TC_RUN(mixed_following_arg); return tc_end(); }
