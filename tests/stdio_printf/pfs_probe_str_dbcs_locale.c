/* TC
 * id:    pfs_probe_str_dbcs_locale
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs setlocale (locale names, code page .932), sprintf %lc/%ls convert via wctomb in the current locale
 * note:  PROBE: Japanese_Japan.932 must be accepted by setlocale (real Windows: yes, code page 932 is built in; Wine may lack it -> skip).
 *        Probes that %lc U+3042 gives lead/trail bytes 82 A0 (return 2), %ls converts mixed wide text, %s passes DBCS bytes through.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile wchar_t v_wc = 0x3042;
static const wchar_t *volatile v_ws = L"\x3042" L"a";
static const char *volatile v_s = "\x82\xA0" "a";

TC_CASE(lc_hiragana) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("Japanese_Japan.932 not available");
  n = sprintf((char *)b, "%lc", (wint_t)v_wc);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 2); CHECK(b[0] == 0x82 && b[1] == 0xA0 && b[2] == 0);
}
TC_CASE(ls_hiragana_a) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("Japanese_Japan.932 not available");
  n = sprintf((char *)b, "%ls", v_ws);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 3); CHECK(b[0] == 0x82 && b[1] == 0xA0 && b[2] == 0x61 && b[3] == 0);
}
TC_CASE(s_bytes_unchanged) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("Japanese_Japan.932 not available");
  n = sprintf((char *)b, "%s", v_s);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 3); CHECK(b[0] == 0x82 && b[1] == 0xA0 && b[2] == 0x61 && b[3] == 0);
}
TC_CASE(restored_c_locale) {
  char b[16]; int n;
  setlocale(LC_ALL, "C");
  n = sprintf(b, "%lc", (wint_t)L'A'); CHECK_EQ_I(n, 1); CHECK_STR(b, "A");
}

int main(void) { tc_begin(); TC_RUN(lc_hiragana); TC_RUN(ls_hiragana_a); TC_RUN(s_bytes_unchanged); TC_RUN(restored_c_locale); return tc_end(); }
