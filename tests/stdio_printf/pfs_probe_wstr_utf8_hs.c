/* TC
 * id:    pfs_probe_wstr_utf8_hs
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs UTF-8 support in UCRT (setlocale(LC_ALL, ".UTF-8")); %hs in wide printf converts via mbrtowc
 * note:  OBSERVED for case hs_emoji (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "hs_emoji n == 2 got=-1"; zzp_probe4 ".UTF-8 -> English_United States.utf8", "hs emoji: r=-1
 *        errno=0 w=[0xfffd]", "hs 2byte: r=1 w=[0xe9]", "hc k: r=1"): in the UTF-8 locale %hs decodes a 2-byte sequence to one wchar_t (U+00E9) but a
 *        4-byte sequence (U+1F600) is NOT turned into a surrogate pair: the conversion yields U+FFFD and the call fails with -1 (errno untouched).
 *        Windows deviates from C11 7.29.2.1 / 7.29.6.3.2 (mbrtowc into a 16-bit wchar_t cannot represent the character; classes B). Cases
 *        hs_two_byte and hc_ascii unchanged. Skips if the locale is unavailable; the .UTF-8 locale exists on 19045 (build-dependent: needs the UTF-8
 *        locale support of Windows 10 1903+). Stable.
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
  CHECK_EQ_I(n, -1); CHECK(b[0] == 0xFFFD);   /* observed: U+1F600 cannot be one 16-bit wchar_t */
}
TC_CASE(hc_ascii) {
  wchar_t b[16] = {0}; int n;
  if (!setlocale(LC_ALL, ".UTF-8")) TC_SKIP("UTF-8 locale unavailable");
  n = _snwprintf(b, 16, L"%hc", 'k');
  setlocale(LC_ALL, "C");
  CHECK_EQ_I(n, 1); CHECK(b[0] == L'k' && b[1] == 0);
}

int main(void) { tc_begin(); TC_RUN(hs_two_byte); TC_RUN(hs_emoji); TC_RUN(hc_ascii); return tc_end(); }
