/* TC
 * id:    pff_f_exact_large_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.1 f conversion; MS docs printf
 * note:  digits beyond DECIMAL_DIG are not mandated exact by ISO C (7.21.6.1 allows rounding either way); UCRT exact expansion is observed behaviour
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static char b[512];
static void chk(double v, const char *want) { volatile double x = v; sprintf(b, "%.0f", x); CHECK_STR(b, want); }

TC_CASE(p53) { chk(9007199254740992.0, "9007199254740992"); }
TC_CASE(p63) { chk(9223372036854775808.0, "9223372036854775808"); }
TC_CASE(p64) { chk(18446744073709551616.0, "18446744073709551616"); }
TC_CASE(e22) { chk(1e22, "10000000000000000000000"); }
TC_CASE(e23) { chk(1e23, "99999999999999991611392"); }
int main(void) { tc_begin(); TC_RUN(p53); TC_RUN(p63); TC_RUN(p64); TC_RUN(e22); TC_RUN(e23); return tc_end(); }
