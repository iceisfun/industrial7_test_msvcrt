/* TC
 * id:    loc_setlocale_failed_keeps_state
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.11.1.1
 * note:  Positive framing; negative twin is loc_neg_setlocale_invalid_name
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;
#define EN "English_United States.1252"

TC_CASE(set_english) { v_name = EN; CHECK_STR(setlocale(LC_ALL, v_name), EN); }
TC_CASE(bad_name_returns_null) { v_name = "zz_nowhere"; CHECK(setlocale(LC_ALL, v_name) == NULL); }
TC_CASE(query_still_english) { CHECK_STR(setlocale(LC_ALL, NULL), EN); CHECK_STR(setlocale(LC_CTYPE, NULL), EN); }
TC_CASE(c_locale_equivalent_case)
{
    v_name = "C"; setlocale(LC_ALL, v_name);
    v_name = "zz_nowhere"; CHECK(setlocale(LC_CTYPE, v_name) == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(set_english); TC_RUN(bad_name_returns_null); TC_RUN(query_still_english); TC_RUN(c_locale_equivalent_case); setlocale(LC_ALL, "C"); return tc_end(); }
