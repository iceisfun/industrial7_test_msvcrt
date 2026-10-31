/* TC
 * id:    pfs_swprintf_s_str_ok
 * area:  stdio_printf
 * funcs: swprintf_s
 * crt:   >=100
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s; printf type field characters (%s wide in wprintf, %hs/%S narrow, %c wide)
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile v_ws = L"wide";
static const char *volatile v_s = "narrow";
static volatile int v_c = L'Z';

TC_CASE(wide_s) {
  wchar_t b[32]; int n;
  n = swprintf_s(b, 32, L"<%s>", v_ws);
  CHECK_EQ_I(n, 6); CHECK_WSTR(b, L"<wide>");
}
TC_CASE(narrow_hs_S) {
  wchar_t b[32]; int n;
  n = swprintf_s(b, 32, L"%hs|%S|%ls", v_s, v_s, v_ws);
  CHECK_EQ_I(n, 18); CHECK_WSTR(b, L"narrow|narrow|wide");
}
TC_CASE(char_c) {
  wchar_t b[32]; int n;
  n = swprintf_s(b, 32, L"%c%lc%hc", v_c, (wint_t)L'Y', 'X');
  CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"ZYX");
}
TC_CASE(exact_fit) {
  wchar_t b[5]; int n;
  n = swprintf_s(b, 5, L"%s", L"abcd");
  CHECK_EQ_I(n, 4); CHECK_WSTR(b, L"abcd");
}
TC_CASE(precision_width) {
  wchar_t b[32]; int n;
  n = swprintf_s(b, 32, L"[%.2s][%5hs]", v_ws, v_s);
  CHECK_WSTR(b, L"[wi][narrow]"); CHECK_EQ_I(n, 12);
}

int main(void) { tc_begin(); TC_RUN(wide_s); TC_RUN(narrow_hs_S); TC_RUN(char_c); TC_RUN(exact_fit); TC_RUN(precision_width); return tc_end(); }
