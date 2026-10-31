/* TC
 * id:    pfq_sprintf_p_star_precision
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  Precision taken from a positional argument (.*m$). Negative precision is in pfq_probe_sprintf_p_edge_cases.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$.*2$s";
static const char * volatile f2 = "%1$.*2$d";
static const char * volatile f3 = "%1$.*2$f";
static const char * volatile f4 = "%1$*2$.*3$f";

TC_CASE(string) { char b[32]; _sprintf_p(b, sizeof b, f1, "abcdef", 3); CHECK_STR(b, "abc"); }
TC_CASE(integer) { char b[32]; _sprintf_p(b, sizeof b, f2, 7, 4); CHECK_STR(b, "0007"); }
TC_CASE(fixed) { char b[32]; double v = 3.14159; _sprintf_p(b, sizeof b, f3, v, 2); CHECK_STR(b, "3.14"); }
TC_CASE(width_and_prec) { char b[32]; double v = 3.14159; int n = _sprintf_p(b, sizeof b, f4, v, 8, 2); CHECK_STR(b, "    3.14"); CHECK_EQ_I(n, 8); }

int main(void) { tc_begin(); TC_RUN(string); TC_RUN(integer); TC_RUN(fixed); TC_RUN(width_and_prec); return tc_end(); }
