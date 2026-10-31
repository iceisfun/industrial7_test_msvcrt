/* TC
 * id:    loc_setlocale_mixed_categories
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs setlocale: composite return format for LC_ALL when categories differ
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

TC_CASE(set_ctype_only) { v_name = EN; CHECK_STR(setlocale(LC_CTYPE, v_name), EN); CHECK_STR(setlocale(LC_CTYPE, NULL), EN); }
TC_CASE(other_categories_remain_C)
{
    CHECK_STR(setlocale(LC_COLLATE, NULL), "C"); CHECK_STR(setlocale(LC_MONETARY, NULL), "C");
    CHECK_STR(setlocale(LC_NUMERIC, NULL), "C"); CHECK_STR(setlocale(LC_TIME, NULL), "C");
}
TC_CASE(composite_string_format)
{
    CHECK_STR(setlocale(LC_ALL, NULL), "LC_COLLATE=C;LC_CTYPE=" EN ";LC_MONETARY=C;LC_NUMERIC=C;LC_TIME=C");
}
TC_CASE(restore) { v_name = "C"; CHECK_STR(setlocale(LC_ALL, v_name), "C"); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(set_ctype_only); TC_RUN(other_categories_remain_C); TC_RUN(composite_string_format); TC_RUN(restore); setlocale(LC_ALL, "C"); return tc_end(); }
