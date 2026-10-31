/* TC
 * id:    loc_neg_free_locale_double_independent
 * area:  locale
 * funcs: _create_locale _free_locale _get_current_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _create_locale, _free_locale, _get_current_locale (returned object must be freed with _free_locale)
 * note:  Freeing one locale object must not affect other independent objects nor the global locale.
 */
#include "tc.h"
#include <locale.h>
#include <ctype.h>
#include <string.h>

static volatile int v_A = 'A';
static const char *volatile v_c = "C";

TC_CASE(free_one_other_survives)
{
    _locale_t a = _create_locale(LC_ALL, v_c), b = _create_locale(LC_ALL, v_c);
    if (!a || !b) TC_SKIP("no locale");
    tc_iph_install(); tc_iph_count = 0;
    _free_locale(a);
    CHECK(_isalpha_l(v_A, b) != 0);
    _free_locale(b);
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(free_snapshot_global_intact)
{
    char before[256]; const char *q; _locale_t s;
    setlocale(LC_ALL, "C");
    q = setlocale(LC_ALL, NULL); CHECK(q != NULL); if (!q) return;
    strncpy(before, q, 255); before[255] = 0;
    s = _get_current_locale(); if (!s) TC_SKIP("no snapshot");
    tc_iph_install(); tc_iph_count = 0;
    _free_locale(s);
    CHECK(isalpha(v_A) != 0);
    CHECK_STR(setlocale(LC_ALL, NULL), before);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(free_one_other_survives); TC_RUN(free_snapshot_global_intact); setlocale(LC_ALL, "C"); return tc_end(); }
