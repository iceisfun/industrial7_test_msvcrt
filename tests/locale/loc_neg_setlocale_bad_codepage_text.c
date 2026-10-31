/* TC
 * id:    loc_neg_setlocale_bad_codepage_text
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   reasoning
 * note:  PROBE: a valid language_country followed by a non-numeric, non-ACP/OCP codepage text is rejected (NULL).
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

TC_CASE(alpha_codepage_null) { v_name = "English_United States.abc"; CHECK(setlocale(LC_ALL, v_name) == NULL); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
TC_CASE(empty_codepage_null) { v_name = "English_United States."; setlocale(LC_ALL, v_name); setlocale(LC_ALL, "C"); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(alpha_codepage_null); TC_RUN(empty_codepage_null); setlocale(LC_ALL, "C"); return tc_end(); }
