/* TC
 * id:    pff_f_dblmax_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.21.6.1 f conversion (exact decimal value); MS docs printf
 * note:  exact digits of a 309-digit value exceed C's DECIMAL_DIG guarantee; UCRT exactness is observed behaviour (VS2015+), not mandated by ISO C; UCRT prints exact decimal expansion; DBL_MAX = 2^1024-2^971
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static volatile double v_max = 1.7976931348623157e308;
static char big[1024];

TC_CASE(length) { int n = sprintf(big, "%.0f", v_max); CHECK_EQ_I(n, 309); CHECK_EQ_U(strlen(big), 309); }
TC_CASE(prefix) { sprintf(big, "%.0f", v_max); CHECK(memcmp(big, "17976931348623157", 17) == 0); }
TC_CASE(suffix) { size_t n; sprintf(big, "%.0f", v_max); n = strlen(big); CHECK_STR(big + n - 6, "858368"); }
TC_CASE(negative) { double m = -v_max; int n = sprintf(big, "%.0f", m); CHECK_EQ_I(n, 310); CHECK(big[0] == '-'); CHECK(memcmp(big + 1, "17976931348623157", 17) == 0); }
int main(void) { tc_begin(); TC_RUN(length); TC_RUN(prefix); TC_RUN(suffix); TC_RUN(negative); return tc_end(); }
