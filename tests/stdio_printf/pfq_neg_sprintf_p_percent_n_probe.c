/* TC
 * id:    pfq_neg_sprintf_p_percent_n_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_p _get_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _set_printf_count_output (%n disabled by default); positional %n in _sprintf_p is not documented separately
 * note:  PROBES the _p variants of the %n rule (the generic doc says only that %n is off by default): plain "ab%n" disabled -> -1, handler, target unmodified.
 *        Positional variant of the %n rule: disabled -> returns -1, invalid-parameter handler invoked, target unmodified;
 *        The enabled case lives in pfq_sprintf_p_percent_n_enabled_probe so that a fault there cannot mask these.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int cp(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap);
    return r;
}
static int def_zero;

TC_CASE(plain_n_disabled)        /* probe */
{
    char b[32]; int n = 12345, r; const char * volatile f = "ab%n";
    if (!def_zero) TC_SKIP("count output enabled");
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, &n);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK_EQ_I(n, 12345);
}
TC_CASE(positional_n_disabled)   /* probe */
{
    char b[32]; int n = 12345, r; const char * volatile f = "ab%1$ncd";
    if (!def_zero) TC_SKIP("count output enabled");
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, &n);
    CHECK_EQ_I(r, -1);
    CHECK(tc_iph_count >= 1);
    CHECK_EQ_I(n, 12345);
}
TC_CASE(guard) { def_zero = (_get_printf_count_output() == 0); if (!def_zero) tc_write("TC SKIP-INFO count output enabled by default\n"); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(guard); TC_RUN(plain_n_disabled); TC_RUN(positional_n_disabled);
    return tc_end();
}
