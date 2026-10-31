/* TC
 * id:    loc_probe_localeconv_en_us_int_curr_and_sign_posn
 * area:  locale
 * funcs: setlocale localeconv
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs localeconv
 * note:  PROBE: en-US int_curr_symbol is "USD " (trailing space), n_cs_precedes 1. n_sign_posn varies by Windows version; it is only recorded (as a skip-note), not asserted.
 */
#include "tc.h"
#include <locale.h>

static int enter(void) { volatile const char *n = "English_United States.1252"; return setlocale(LC_ALL, (const char *)n) != NULL; }

TC_CASE(int_curr_symbol_USD_space) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_STR(localeconv()->int_curr_symbol, "USD "); setlocale(LC_ALL, "C"); }
TC_CASE(n_cs_precedes_1) { if (!enter()) TC_SKIP("locale unavailable"); CHECK_EQ_I(localeconv()->n_cs_precedes, 1); setlocale(LC_ALL, "C"); }
TC_CASE(n_sign_posn_value_recorded)
{
    int v;
    char b[32], m[96] = "TC INFO n_sign_posn=";
    int i = 0, j = 0;
    if (!enter()) TC_SKIP("locale unavailable");
    v = localeconv()->n_sign_posn;
    setlocale(LC_ALL, "C");
    tc_i64(v, b);
    while (m[i]) i++;
    while (b[j]) m[i++] = b[j++];
    m[i++] = '\n'; m[i] = 0;
    tc_write(m);
    CHECK(v >= 0 && v <= 4);
}
int main(void) { tc_begin(); TC_RUN(int_curr_symbol_USD_space); TC_RUN(n_cs_precedes_1); TC_RUN(n_sign_posn_value_recorded); return tc_end(); }
