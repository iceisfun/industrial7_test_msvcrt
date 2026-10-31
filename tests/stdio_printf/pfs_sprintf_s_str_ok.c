/* TC
 * id:    pfs_sprintf_s_str_ok
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, sprintf_s_l, swprintf_s, swprintf_s_l; printf type field characters (%s %S %c %C)
 * note:  successful sprintf_s string cases; msvcr90/80 not listed (mingw import libs there lack or differ).
 */
#include "tc.h"
#include <stdio.h>

static const char *volatile v_s = "hello";
static const wchar_t *volatile v_ws = L"wide";
static volatile int v_c = 'Z';
static volatile wchar_t v_wc = L'Q';

TC_CASE(fits_with_room) {
  char b[32]; int n;
  b[10] = 'X';
  n = sprintf_s(b, sizeof b, "<%s>", v_s);
  CHECK_EQ_I(n, 7); CHECK_STR(b, "<hello>"); CHECK(b[7] == 0);
}
TC_CASE(exact_size_minus_one) {
  char b[6]; int n;
  n = sprintf_s(b, sizeof b, "%s", v_s);   /* 5 chars + NUL == 6 */
  CHECK_EQ_I(n, 5); CHECK_STR(b, "hello");
}
TC_CASE(wide_and_char) {
  char b[64]; int n;
  n = sprintf_s(b, sizeof b, "%ls|%S|%c|%lc", v_ws, v_ws, v_c, v_wc);
  CHECK_STR(b, "wide|wide|Z|Q"); CHECK_EQ_I(n, 13);
}
TC_CASE(precision) {
  char b[32]; int n;
  n = sprintf_s(b, sizeof b, "[%.2s][%.0s][%.10s]", v_s, v_s, v_s);
  CHECK_STR(b, "[he][][hello]"); CHECK_EQ_I(n, 13);
}
TC_CASE(width) {
  char b[32]; int n;
  n = sprintf_s(b, sizeof b, "[%7s][%-7s]", v_s, v_s);
  CHECK_STR(b, "[  hello][hello  ]"); CHECK_EQ_I(n, 18);
}

int main(void) { tc_begin(); TC_RUN(fits_with_room); TC_RUN(exact_size_minus_one); TC_RUN(wide_and_char); TC_RUN(precision); TC_RUN(width); return tc_end(); }
