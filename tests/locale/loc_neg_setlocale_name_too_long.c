/* TC
 * id:    loc_neg_setlocale_name_too_long
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   reasoning
 * note:  PROBE: crash-free NULL return for a 400-char locale name.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

TC_CASE(long_name_null)
{
    static char n[401]; int i; for (i = 0; i < 400; i++) n[i] = 'a'; n[400] = 0;
    v_name = n; CHECK(setlocale(LC_ALL, v_name) == NULL);
}
TC_CASE(locale_unchanged) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(long_name_null); TC_RUN(locale_unchanged); setlocale(LC_ALL, "C"); return tc_end(); }
