/* TC
 * id:    loc_setlocale_english_us_1252
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs setlocale: language_country.codepage form
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;
#define EN "English_United States.1252"

TC_CASE(set_returns_full_name) { v_name = EN; CHECK_STR(setlocale(LC_ALL, v_name), EN); }
TC_CASE(query_LC_CTYPE_same) { v_name = EN; setlocale(LC_ALL, v_name); CHECK_STR(setlocale(LC_CTYPE, NULL), EN); CHECK_STR(setlocale(LC_NUMERIC, NULL), EN); }
TC_CASE(query_LC_ALL_same) { v_name = EN; setlocale(LC_ALL, v_name); CHECK_STR(setlocale(LC_ALL, NULL), EN); }
TC_CASE(restore_C) { v_name = "C"; CHECK_STR(setlocale(LC_ALL, v_name), "C"); CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
int main(void) { tc_begin(); TC_RUN(set_returns_full_name); TC_RUN(query_LC_CTYPE_same); TC_RUN(query_LC_ALL_same); TC_RUN(restore_C); setlocale(LC_ALL, "C"); return tc_end(); }
