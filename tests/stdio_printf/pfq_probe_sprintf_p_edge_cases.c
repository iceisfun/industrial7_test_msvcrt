/* TC
 * id:    pfq_probe_sprintf_p_edge_cases
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  PROBES standard-C edge behaviour combined with positional syntax: negative '*' width acts as '-' flag then abs width; negative '.*' precision is treated as omitted; width arg also used as data (%1$*1$d); precision 0 with value 0 prints no digits. Whether MS positional parsing honours these is unverified.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$*2$d|";
static const char * volatile f2 = "%1$.*2$d";
static const char * volatile f3 = "%1$*1$d";
static const char * volatile f4 = "%1$.0d";

TC_CASE(negative_width) { char b[32]; _sprintf_p(b, sizeof b, f1, 42, -6); CHECK_STR(b, "42    |"); }
TC_CASE(negative_precision) { char b[32]; _sprintf_p(b, sizeof b, f2, 7, -1); CHECK_STR(b, "7"); }
TC_CASE(width_is_data) { char b[32]; _sprintf_p(b, sizeof b, f3, 3); CHECK_STR(b, "  3"); }
TC_CASE(zero_prec_zero) { char b[32]; int n = _sprintf_p(b, sizeof b, f4, 0); CHECK_STR(b, ""); CHECK_EQ_I(n, 0); }

int main(void) { tc_begin(); TC_RUN(negative_width); TC_RUN(negative_precision); TC_RUN(width_is_data); TC_RUN(zero_prec_zero); return tc_end(); }
