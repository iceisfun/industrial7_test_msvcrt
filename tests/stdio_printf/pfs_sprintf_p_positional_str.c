/* TC
 * id:    pfs_sprintf_p_positional_str
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _printf_p, _sprintf_p (positional parameters, n$)
 * note:  msvcr80 omitted (mingw import lib lacks _sprintf_p).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile a = "one", *volatile b2 = "two";

TC_CASE(swap) { char b[64]; int n = _sprintf_p(b, sizeof b, "%2$s %1$s", a, b2); CHECK_STR(b, "two one"); CHECK_EQ_I(n, 7); }
TC_CASE(reuse) { char b[64]; int n = _sprintf_p(b, sizeof b, "%1$s-%1$s", a); CHECK_STR(b, "one-one"); CHECK_EQ_I(n, 7); }
TC_CASE(mix_narrow_wide) {
  char b[64]; int n;
  n = _sprintf_p(b, sizeof b, "%1$c%2$ls", (int)'x', L"yz");
  CHECK_STR(b, "xyz"); CHECK_EQ_I(n, 3);
}
TC_CASE(width) { char b[64]; int n = _sprintf_p(b, sizeof b, "[%1$5s]", a); CHECK_STR(b, "[  one]"); CHECK_EQ_I(n, 7); }

int main(void) { tc_begin(); TC_RUN(swap); TC_RUN(reuse); TC_RUN(mix_narrow_wide); TC_RUN(width); return tc_end(); }
