/* TC
 * id:    loc_neg_setlocale_invalid_name
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale: returns NULL if the input is not valid
 * note:  BCP-47-style names (e.g. xx-YY) are deliberately not asserted here (ucrt may accept them; see loc_probe_setlocale_bcp47_names).
 *        Does not test the empty string (user default). Bad codepage text is in loc_neg_setlocale_bad_codepage_text (probe).
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

static void bad(const char *n)
{
    v_name = n;
    CHECK(setlocale(LC_ALL, v_name) == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
TC_CASE(garbage_null) { bad("garbage"); bad("zz_nowhere"); bad("\x7f"); }
TC_CASE(unknown_language_null) { bad("Klingonese_United States.1252"); }
TC_CASE(unknown_country_null) { bad("English_Nowhere.1252"); }
TC_CASE(state_unchanged_after_each)
{
    v_name = "zz_nowhere"; CHECK(setlocale(LC_CTYPE, v_name) == NULL); CHECK(setlocale(LC_NUMERIC, v_name) == NULL);
    CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(garbage_null); TC_RUN(unknown_language_null); TC_RUN(unknown_country_null); TC_RUN(state_unchanged_after_each); setlocale(LC_ALL, "C"); return tc_end(); }
