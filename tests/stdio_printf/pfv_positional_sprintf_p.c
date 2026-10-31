/* TC
 * id:    pfv_positional_sprintf_p
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf_p positional parameters
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(reorder) { char b[64]; volatile const char *a = "a", *c = "b"; _sprintf_p(b, 64, "%2$s %1$s", a, c); CHECK_STR(b, "b a"); }
TC_CASE(repeat_arg) { char b[64]; volatile int v = 7; _sprintf_p(b, 64, "%1$d %1$d", v); CHECK_STR(b, "7 7"); }
TC_CASE(int_and_str) { char b[64]; volatile int v = 42; volatile const char *s = "xy"; int n = _sprintf_p(b, 64, "%2$s=%1$d", v, s); CHECK_STR(b, "xy=42"); CHECK_EQ_I(n, 5); }

int main(void) { tc_begin(); TC_RUN(reorder); TC_RUN(repeat_arg); TC_RUN(int_and_str); return tc_end(); }
