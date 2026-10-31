/* TC
 * id:    pff_f_locale_decimal_comma_probe
 * area:  stdio_printf
 * funcs: _sprintf_l
 * procs: _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _create_locale, _sprintf_l; locale names ("de-DE" accepted by UCRT)
 * note:  PROBE: German locale uses ',' as the decimal separator in %f and %e through _sprintf_l. TC_SKIP when the locale is unavailable.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_DECL_PROC(void *, _create_locale, (int, const char *));
TC_DECL_PROC(void, _free_locale, (void *));

TC_CASE(f_comma) { char b[32]; volatile double v = 1.5; void *loc;
    if (!tc_p__create_locale) TC_SKIP("none"); loc = tc_p__create_locale(LC_ALL, "de-DE"); if (!loc) TC_SKIP("de-DE unavailable");
    _sprintf_l(b, "%.2f", loc, v); CHECK_STR(b, "1,50"); tc_p__free_locale(loc); }
TC_CASE(e_comma) { char b[32]; volatile double v = 1.5; void *loc;
    if (!tc_p__create_locale) TC_SKIP("none"); loc = tc_p__create_locale(LC_ALL, "de-DE"); if (!loc) TC_SKIP("de-DE unavailable");
    _sprintf_l(b, "%.2e", loc, v); CHECK_STR(b, "1,50e+00"); tc_p__free_locale(loc); }
TC_CASE(null_stays_c) { char b[32]; volatile double v = 1.5; _sprintf_l(b, "%.2f", NULL, v); CHECK_STR(b, "1.50"); }
int main(void) { tc_begin(); TC_LOAD_PROC(_create_locale); TC_LOAD_PROC(_free_locale); TC_RUN(f_comma); TC_RUN(e_comma); TC_RUN(null_stays_c); return tc_end(); }
