/* TC
 * id:    loc_neg_setlocale_unsupported_codepages
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs setlocale: code pages needing more than two bytes per char (UTF-7, UTF-8 on old CRTs) are not supported
 * note:  Only UTF-7 (.65000) is documented. UTF-16/32 and bogus code pages are in loc_neg_probe_setlocale_other_codepages.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

static void nul(const char *n) { v_name = n; CHECK(setlocale(LC_ALL, v_name) == NULL); }
TC_CASE(utf7_null) { nul(".65000"); }
TC_CASE(locale_unchanged) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(utf7_null); TC_RUN(locale_unchanged); setlocale(LC_ALL, "C"); return tc_end(); }
