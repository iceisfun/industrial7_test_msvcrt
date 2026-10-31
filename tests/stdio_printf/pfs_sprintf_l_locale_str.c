/* TC
 * id:    pfs_sprintf_l_locale_str
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, sprintf_l; _create_locale
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static const char *volatile s = "abc";
static const wchar_t *volatile ws = L"wide";

TC_CASE(explicit_c_locale) {
  _locale_t L = _create_locale(LC_ALL, "C"); char b[64]; int n;
  CHECK(L != NULL); if (!L) return;
  n = _sprintf_l(b, "%s|%lc|%ls", L, s, (wint_t)L'A', ws);
  CHECK_STR(b, "abc|A|wide"); CHECK_EQ_I(n, 10);
  _free_locale(L);
}
TC_CASE(null_locale_is_current) {
  char b[64]; int n = _sprintf_l(b, "%s-%c", NULL, s, 'q');
  CHECK_STR(b, "abc-q"); CHECK_EQ_I(n, 5);
}

int main(void) { tc_begin(); TC_RUN(explicit_c_locale); TC_RUN(null_locale_is_current); return tc_end(); }
