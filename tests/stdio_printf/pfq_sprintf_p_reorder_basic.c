/* TC
 * id:    pfq_sprintf_p_reorder_basic
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_swap = "%2$d %1$d";
static const char * volatile f_rev = "%3$s-%2$s-%1$s";
static const char * volatile f_id = "%1$d%2$d%3$d";
static const char * volatile f_mix = "%2$s=%1$d";

TC_CASE(swap2) { char b[32]; int n = _sprintf_p(b, sizeof b, f_swap, 1, 2); CHECK_STR(b, "2 1"); CHECK_EQ_I(n, 3); }
TC_CASE(reverse3) { char b[32]; int n = _sprintf_p(b, sizeof b, f_rev, "a", "b", "c"); CHECK_STR(b, "c-b-a"); CHECK_EQ_I(n, 5); }
TC_CASE(identity) { char b[32]; int n = _sprintf_p(b, sizeof b, f_id, 1, 2, 3); CHECK_STR(b, "123"); CHECK_EQ_I(n, 3); }
TC_CASE(mixed_types) { char b[32]; int n = _sprintf_p(b, sizeof b, f_mix, 5, "x"); CHECK_STR(b, "x=5"); CHECK_EQ_I(n, 3); }

int main(void) { tc_begin(); TC_RUN(swap2); TC_RUN(reverse3); TC_RUN(identity); TC_RUN(mixed_types); return tc_end(); }
