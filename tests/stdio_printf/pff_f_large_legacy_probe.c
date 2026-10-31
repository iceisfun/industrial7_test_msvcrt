/* TC
 * id:    pff_f_large_legacy_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   win7
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs printf (legacy CRT)
 * note:  probed: legacy CRT keeps 17 significant digits then zero-fills; values below 1e16 stay exact
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static char b[512];
static void chk(double v, const char *want) { volatile double x = v; sprintf(b, "%.0f", x); CHECK_STR(b, want); }

TC_CASE(p53) { chk(9007199254740992.0, "9007199254740992"); }
TC_CASE(e15) { chk(1e15, "1000000000000000"); }
TC_CASE(p63) { chk(9223372036854775808.0, "9223372036854775800"); }
TC_CASE(e23) { chk(1e23, "99999999999999992000000"); }
int main(void) { tc_begin(); TC_RUN(p53); TC_RUN(e15); TC_RUN(p63); TC_RUN(e23); return tc_end(); }
