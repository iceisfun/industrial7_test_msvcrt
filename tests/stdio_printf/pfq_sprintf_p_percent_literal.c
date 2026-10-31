/* TC
 * id:    pfq_sprintf_p_percent_literal
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f1 = "%1$d%%";
static const char * volatile f2 = "%%%1$d";
static const char * volatile f3 = "100%% of %1$s";
static const char * volatile f4 = "cost $%1$d";
static const char * volatile f5 = "[%2$s|%1$s]";

TC_CASE(trailing) { char b[32]; int n = _sprintf_p(b, sizeof b, f1, 50); CHECK_STR(b, "50%"); CHECK_EQ_I(n, 3); }
TC_CASE(leading) { char b[32]; _sprintf_p(b, sizeof b, f2, 50); CHECK_STR(b, "%50"); }
TC_CASE(middle) { char b[32]; _sprintf_p(b, sizeof b, f3, "x"); CHECK_STR(b, "100% of x"); }
TC_CASE(dollar_literal) { char b[32]; _sprintf_p(b, sizeof b, f4, 5); CHECK_STR(b, "cost $5"); }
TC_CASE(brackets) { char b[32]; _sprintf_p(b, sizeof b, f5, "a", "b"); CHECK_STR(b, "[b|a]"); }

int main(void) { tc_begin(); TC_RUN(trailing); TC_RUN(leading); TC_RUN(middle); TC_RUN(dollar_literal); TC_RUN(brackets); return tc_end(); }
