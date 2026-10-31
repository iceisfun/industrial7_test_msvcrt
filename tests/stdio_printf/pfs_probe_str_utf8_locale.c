/* TC
 * id:    pfs_probe_str_utf8_locale
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs Locale names/UTF-8 support (UCRT, Windows 10 1803+): setlocale(LC_ALL, ".UTF-8")
 * note:  PROBE: with a UTF-8 locale %lc / %ls convert wide chars to UTF-8 multibyte output. One case per assertion;
 *        the surrogate-pair case in particular is uncertain (printf may convert per wchar_t and fail on lone surrogates).
 *        Skips if setlocale(".UTF-8") fails (older Windows builds).
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile wchar_t v_wc = 0xE9;
static const wchar_t *volatile v_pair = L"\xD83D\xDE00";
static const wchar_t *volatile v_euro = L"\x20AC";

TC_CASE(lc_e_acute) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = sprintf((char *)b, "%lc", (wint_t)v_wc);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 2); CHECK(b[0] == 0xC3 && b[1] == 0xA9 && b[2] == 0);
}
TC_CASE(ls_surrogate_pair) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = sprintf((char *)b, "%ls", v_pair);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 4); CHECK(b[0] == 0xF0 && b[1] == 0x9F && b[2] == 0x98 && b[3] == 0x80 && b[4] == 0);
}
TC_CASE(ls_euro) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = sprintf((char *)b, "%ls", v_euro);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 3); CHECK(b[0] == 0xE2 && b[1] == 0x82 && b[2] == 0xAC && b[3] == 0);
}

int main(void) { tc_begin(); TC_RUN(lc_e_acute); TC_RUN(ls_surrogate_pair); TC_RUN(ls_euro); return tc_end(); }
