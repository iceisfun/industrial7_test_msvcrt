/* TC
 * id:    loc_setlocale_codepage_only
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs setlocale: .codepage, .ACP, .OCP forms
 * note:  .ACP/.OCP are skipped on old CRTs when the system code page is 65001. Do not assert the language part (depends on user default).
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

TC_CASE(dot_1252_nonnull) { v_name = ".1252"; CHECK(setlocale(LC_ALL, v_name) != NULL); setlocale(LC_ALL, "C"); }
TC_CASE(dot_1252_suffix) { v_name = ".1252"; char *r = setlocale(LC_ALL, v_name); CHECK(r != NULL); CHECK(ends(r, ".1252")); setlocale(LC_ALL, "C"); }
TC_CASE(dot_ACP_nonnull) { if (TC_BELOW(140) && GetACP() == 65001) TC_SKIP("UTF-8 ACP unsupported by this CRT"); v_name = ".ACP"; CHECK(setlocale(LC_ALL, v_name) != NULL); setlocale(LC_ALL, "C"); }
TC_CASE(dot_OCP_nonnull) { if (TC_BELOW(140) && GetOEMCP() == 65001) TC_SKIP("UTF-8 OCP unsupported by this CRT"); v_name = ".OCP"; CHECK(setlocale(LC_ALL, v_name) != NULL); setlocale(LC_ALL, "C"); }
int main(void) { tc_begin(); TC_RUN(dot_1252_nonnull); TC_RUN(dot_1252_suffix); TC_RUN(dot_ACP_nonnull); TC_RUN(dot_OCP_nonnull); return tc_end(); }
