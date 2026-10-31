/* TC
 * id:    pff_sprintf_l_float
 * area:  stdio_printf
 * funcs: _sprintf_l _sprintf_s_l
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, sprintf_s, _sprintf_s_l; _create_locale ("C" locale)
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));

TC_CASE(null_locale) { char b[32]; volatile double v = 1.5; int n = _sprintf_l(b, "%.2f", NULL, v); CHECK_STR(b, "1.50"); CHECK_EQ_I(n, 4); }
TC_CASE(s_l)         { char b[32]; volatile double v = 0.25; int n = _sprintf_s_l(b, 32, "%g", NULL, v); CHECK_STR(b, "0.25"); CHECK_EQ_I(n, 4); }
TC_CASE(c_locale)
{
    char b[32]; volatile double v = 1234.5; void *loc; int n;
    if (!tc_p__create_locale || !tc_p__free_locale) TC_SKIP("none");
    loc = tc_p__create_locale(LC_ALL, "C"); CHECK(loc != NULL); if (!loc) return;
    n = _sprintf_l(b, "%.1f|%e", loc, v, v);
    CHECK(b[0] == '1' && b[1] == '2' && b[2] == '3' && b[3] == '4' && b[4] == '.' && b[5] == '5' && b[6] == '|');
    CHECK_EQ_I(b[7], '1'); CHECK_EQ_I(b[8], '.'); CHECK_EQ_I(n, 7 + (TC_AT_LEAST(140) ? 12 : 13));
    tc_p__free_locale(loc);
}
int main(void) { tc_begin(); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_RUN(null_locale); TC_RUN(s_l); TC_RUN(c_locale); return tc_end(); }
