/* TC
 * id:    loc_locale_name_roundtrip_composite
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs setlocale (returned string can be passed back)
 * note:  The exact composite string layout (LC_COLLATE=..;LC_CTYPE=..;LC_MONETARY=..;LC_NUMERIC=..;LC_TIME=..) is known
 *        CRT behaviour, not spelled out field-by-field in the docs.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;
#define EN "English_United States.1252"
#define DE "German_Germany.1252"
static char comp[512];

TC_CASE(capture_composite)
{
    v_name = EN; CHECK(setlocale(LC_CTYPE, v_name) != NULL);
    v_name = DE; CHECK(setlocale(LC_COLLATE, v_name) != NULL);
    cp(comp, setlocale(LC_ALL, NULL), sizeof comp);
    CHECK_STR(comp, "LC_COLLATE=" DE ";LC_CTYPE=" EN ";LC_MONETARY=C;LC_NUMERIC=C;LC_TIME=C");
}
TC_CASE(reset_to_C) { v_name = "C"; CHECK_STR(setlocale(LC_ALL, v_name), "C"); CHECK_STR(setlocale(LC_COLLATE, NULL), "C"); }
TC_CASE(apply_composite_nonnull) { v_name = comp; CHECK(comp[0] != 0); CHECK(setlocale(LC_ALL, v_name) != NULL); }
TC_CASE(per_category_restored)
{
    CHECK_STR(setlocale(LC_CTYPE, NULL), EN); CHECK_STR(setlocale(LC_COLLATE, NULL), DE);
    CHECK_STR(setlocale(LC_MONETARY, NULL), "C"); CHECK_STR(setlocale(LC_NUMERIC, NULL), "C"); CHECK_STR(setlocale(LC_TIME, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(capture_composite); TC_RUN(reset_to_C); TC_RUN(apply_composite_nonnull); TC_RUN(per_category_restored); setlocale(LC_ALL, "C"); return tc_end(); }
