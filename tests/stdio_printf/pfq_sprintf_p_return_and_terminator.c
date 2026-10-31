/* TC
 * id:    pfq_sprintf_p_return_and_terminator
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; _sprintf_p, _sprintf_p_l, _swprintf_p
 * note:  Exact-fit case relies on count including the NUL (size == len+1).
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_d = "%1$d-%2$s";
static const char * volatile f_long = "%1$s%1$s";
static const char * volatile f_empty = "%1$s";

TC_CASE(return_value) { char b[32]; int n = _sprintf_p(b, sizeof b, f_d, 12, "abc"); CHECK_EQ_I(n, 6); CHECK_EQ_I(b[6], 0); }
TC_CASE(exact_fit) { char b[7]; int n = _sprintf_p(b, 7, f_d, 12, "abc"); CHECK_EQ_I(n, 6); CHECK_STR(b, "12-abc"); }
TC_CASE(long_result)
{
    char s[101], b[256]; int i, n;
    for (i = 0; i < 100; i++) s[i] = 'a' + i % 26;
    s[100] = 0;
    n = _sprintf_p(b, sizeof b, f_long, s);
    CHECK_EQ_I(n, 200); CHECK_EQ_I(b[200], 0); CHECK(b[199] == s[99]); CHECK(b[100] == s[0]);
}
TC_CASE(empty_result) { char b[8]; int n; b[0] = 'z'; n = _sprintf_p(b, sizeof b, f_empty, ""); CHECK_EQ_I(n, 0); CHECK_EQ_I(b[0], 0); }
int main(void) { tc_begin(); TC_RUN(return_value); TC_RUN(exact_fit); TC_RUN(long_result); TC_RUN(empty_result); return tc_end(); }
