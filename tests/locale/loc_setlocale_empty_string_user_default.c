/* TC
 * id:    loc_setlocale_empty_string_user_default
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs setlocale: "" = user-default
 * note:  PROBE: user-default locale is machine specific; only that it succeeds and is self-consistent. Name printed in diagnostic.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;
static char got[512];

TC_CASE(empty_nonnull)
{
    v_name = ""; char *r = setlocale(LC_ALL, v_name); CHECK(r != NULL);
    cp(got, r, sizeof got); tc_write("TC INFO user default locale: "); tc_write(got); tc_write("\n");
}
TC_CASE(query_matches) { CHECK(got[0] != 0); CHECK_STR(setlocale(LC_ALL, NULL), got); }
TC_CASE(restore_C) { v_name = "C"; CHECK_STR(setlocale(LC_ALL, v_name), "C"); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(empty_nonnull); TC_RUN(query_matches); TC_RUN(restore_C); setlocale(LC_ALL, "C"); return tc_end(); }
