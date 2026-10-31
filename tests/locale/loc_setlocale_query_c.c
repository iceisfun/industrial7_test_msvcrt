/* TC
 * id:    loc_setlocale_query_c
 * area:  locale
 * funcs: setlocale
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.11.1.1; MS docs setlocale
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
static void cp(char *d, const char *s, int n) { int i = 0; if (!s) { d[0] = 0; return; } while (s[i] && i < n - 1) { d[i] = s[i]; i++; } d[i] = 0; }
static int ends(const char *s, const char *suf) { int a = 0, b = 0; if (!s) return 0; while (s[a]) a++; while (suf[b]) b++; if (b > a) return 0; return tc_strequ(s + a - b, suf); }
static const char * volatile v_name;

TC_CASE(query_all_is_C) { CHECK_STR(setlocale(LC_ALL, NULL), "C"); }
TC_CASE(query_each_category_LC_COLLATE_CTYPE_MONETARY_NUMERIC_TIME)
{
    CHECK_STR(setlocale(LC_COLLATE, NULL), "C"); CHECK_STR(setlocale(LC_CTYPE, NULL), "C");
    CHECK_STR(setlocale(LC_MONETARY, NULL), "C"); CHECK_STR(setlocale(LC_NUMERIC, NULL), "C");
    CHECK_STR(setlocale(LC_TIME, NULL), "C");
}
TC_CASE(set_C_returns_C) { v_name = "C"; CHECK_STR(setlocale(LC_ALL, v_name), "C"); }
TC_CASE(set_C_repeat_idempotent)
{
    v_name = "C";
    CHECK_STR(setlocale(LC_ALL, v_name), "C"); CHECK_STR(setlocale(LC_ALL, v_name), "C"); CHECK_STR(setlocale(LC_ALL, NULL), "C");
}
int main(void) { tc_begin(); TC_RUN(query_all_is_C); TC_RUN(query_each_category_LC_COLLATE_CTYPE_MONETARY_NUMERIC_TIME); TC_RUN(set_C_returns_C); TC_RUN(set_C_repeat_idempotent); return tc_end(); }
