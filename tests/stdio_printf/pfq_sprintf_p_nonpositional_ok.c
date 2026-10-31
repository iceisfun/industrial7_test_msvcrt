/* TC
 * id:    pfq_sprintf_p_nonpositional_ok
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%d %s";
static const char * volatile f2 = "%5.2f";
static const char * volatile f3 = "%%";
static const char * volatile f4 = "";
static const char * volatile f5 = "hello world";

TC_CASE(int_str) { char b[32]; int n = _sprintf_p(b, sizeof b, f1, 3, "x"); CHECK_STR(b, "3 x"); CHECK_EQ_I(n, 3); }
TC_CASE(fixed) { char b[32]; double v = 3.14159; _sprintf_p(b, sizeof b, f2, v); CHECK_STR(b, " 3.14"); }
TC_CASE(percent) { char b[32]; int n = _sprintf_p(b, sizeof b, f3); CHECK_STR(b, "%"); CHECK_EQ_I(n, 1); }
TC_CASE(empty) { char b[32]; int n; b[0] = 'z'; n = _sprintf_p(b, sizeof b, f4); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }
TC_CASE(literal) { char b[32]; int n = _sprintf_p(b, sizeof b, f5); CHECK_STR(b, "hello world"); CHECK_EQ_I(n, 11); }

int main(void) { tc_begin(); TC_RUN(int_str); TC_RUN(fixed); TC_RUN(percent); TC_RUN(empty); TC_RUN(literal); return tc_end(); }
