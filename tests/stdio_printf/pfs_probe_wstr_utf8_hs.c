/* TC
 * id:    pfs_probe_wstr_utf8_hs
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs UTF-8 support in UCRT (setlocale(LC_ALL, ".UTF-8")); %hs in wide printf converts via mbrtowc
 * note:  PROBE: in a UTF-8 locale %hs input is decoded as UTF-8 into wchar_t (2-byte seq -> U+00E9, 4-byte seq -> surrogate pair).
 *        Skips when the locale is unavailable.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <locale.h>

static const char *volatile v_e = "\xC3\xA9";
static const char *volatile v_emoji = "\xF0\x9F\x98\x80";

TC_CASE(hs_two_byte) {
  wchar_t b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = _snwprintf(b, 16, L"%hs", v_e);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 1); CHECK(b[0] == 0x00E9 && b[1] == 0);
}
TC_CASE(hs_emoji) {
  wchar_t b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = _snwprintf(b, 16, L"%hs", v_emoji);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 2); CHECK(b[0] == 0xD83D && b[1] == 0xDE00 && b[2] == 0);
}
TC_CASE(hc_ascii) {
  wchar_t b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = _snwprintf(b, 16, L"%hc", 'k');
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 1); CHECK(b[0] == L'k' && b[1] == 0);
}

int main(void) { tc_begin(); TC_RUN(hs_two_byte); TC_RUN(hs_emoji); TC_RUN(hc_ascii); return tc_end(); }
