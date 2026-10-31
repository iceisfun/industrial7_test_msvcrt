/* TC
 * id:    pfq_sprintf_p_reuse_position
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$d %1$d %1$d";
static const char * volatile f2 = "%1$s/%1$s";
static const char * volatile f3 = "%1$5d|%1$-5d|";
static const char * volatile f4 = "%1$d %1$x %1$o";
static const char * volatile f5 = "%2$d %1$d %2$d";

TC_CASE(thrice) { char b[32]; int n = _sprintf_p(b, sizeof b, f1, 7); CHECK_STR(b, "7 7 7"); CHECK_EQ_I(n, 5); }
TC_CASE(string_twice) { char b[32]; _sprintf_p(b, sizeof b, f2, "ab"); CHECK_STR(b, "ab/ab"); }
TC_CASE(widths) { char b[32]; int n = _sprintf_p(b, sizeof b, f3, 42); CHECK_STR(b, "   42|42   |"); CHECK_EQ_I(n, 12); }
TC_CASE(bases) { char b[32]; _sprintf_p(b, sizeof b, f4, 255); CHECK_STR(b, "255 ff 377"); }
TC_CASE(out_of_order) { char b[32]; _sprintf_p(b, sizeof b, f5, 1, 2); CHECK_STR(b, "2 1 2"); }

int main(void) { tc_begin(); TC_RUN(thrice); TC_RUN(string_twice); TC_RUN(widths); TC_RUN(bases); TC_RUN(out_of_order); return tc_end(); }
