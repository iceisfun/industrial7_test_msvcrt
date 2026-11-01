/* TC
 * id:    pfs_probe_str_utf8_locale
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs Locale names/UTF-8 support (UCRT, Windows 10 1803+): setlocale(LC_ALL, ".UTF-8")
 * note:  PROBE: with a UTF-8 locale %lc / %ls convert wide chars to UTF-8 multibyte output. One case per assertion;
 *        OBSERVED (class B, deviates from C11 7.21.6.1: valid input should convert): the surrogate-pair case crashes ucrtbase with an access violation (0xC0000005) on Win10 22H2 19045; it runs in a child process and asserts that exit code.
 *        Skips if setlocale(".UTF-8") fails (older Windows builds).
 *        B: ls_surrogate_pair now asserts the child's crash exit code 0xC0000005 (deterministic). lc_e_acute and ls_euro unchanged (pass).
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
  /* C: observed on Win10 22H2 19045: %ls of a surrogate pair under ".UTF-8" crashes ucrtbase (access violation) - child process */
  DWORD code = 0;
  CHECK(tc_spawn_self("pair", &code, 20000));
  if (code == 78) TC_SKIP("UTF-8 locale unavailable");
  CHECK_EQ_U(code, TC_STATUS_ACCESS_VIOLATION);
}
TC_CASE(ls_euro) {
  unsigned char b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = sprintf((char *)b, "%ls", v_euro);
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 3); CHECK(b[0] == 0xE2 && b[1] == 0x82 && b[2] == 0xAC && b[3] == 0);
}

int main(void) {
  tc_begin();
  if (tc_child_name()) {
    unsigned char b[16] = {0};
    if (!setlocale(LC_ALL, ".UTF-8")) return 78;
    sprintf((char *)b, "%ls", v_pair);
    return 77;   /* reached only if the CRT did not crash */
  }
  TC_RUN(lc_e_acute); TC_RUN(ls_surrogate_pair); TC_RUN(ls_euro); return tc_end(); }
