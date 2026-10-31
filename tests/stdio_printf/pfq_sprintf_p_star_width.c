/* TC
 * id:    pfq_sprintf_p_star_width
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  Width taken from a positional argument (*m$). Negative / self-referencing cases are in pfq_probe_sprintf_p_edge_cases.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$*2$d";
static const char * volatile f2 = "%1$-*2$d|";
static const char * volatile f3 = "%3$*1$d%2$s";
static const char * volatile f4 = "%1$0*2$d";

TC_CASE(right_justify) { char b[32]; int n = _sprintf_p(b, sizeof b, f1, 42, 6); CHECK_STR(b, "    42"); CHECK_EQ_I(n, 6); }
TC_CASE(left_justify_flag) { char b[32]; _sprintf_p(b, sizeof b, f2, 42, 6); CHECK_STR(b, "42    |"); }
TC_CASE(width_first_value_last) { char b[32]; _sprintf_p(b, sizeof b, f3, 6, "x", 42); CHECK_STR(b, "    42x"); }
TC_CASE(zero_flag) { char b[32]; _sprintf_p(b, sizeof b, f4, 42, 5); CHECK_STR(b, "00042"); }

int main(void) { tc_begin(); TC_RUN(right_justify); TC_RUN(left_justify_flag); TC_RUN(width_first_value_last); TC_RUN(zero_flag); return tc_end(); }
