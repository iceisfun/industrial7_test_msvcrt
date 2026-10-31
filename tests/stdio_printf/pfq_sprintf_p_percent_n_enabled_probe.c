/* TC
 * id:    pfq_sprintf_p_percent_n_enabled_probe
 * area:  stdio_printf
 * kind:  positive
 * funcs: _vsprintf_p _set_printf_count_output _get_printf_count_output
 * crt:   >=90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _set_printf_count_output (enables %n); positional %n in _vsprintf_p is not documented separately
 * note:  PROBES that after _set_printf_count_output(1), '%1$n' in _vsprintf_p stores the character count so far (3 for "abc%1$n"),
 *        returns 3 and does not call the invalid-parameter handler. Setting restored to 0 afterwards. Isolated because it writes through
 *        a pointer taken from the va_list.
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

TC_CASE(positional_n_enabled)
{
    char b[32]; int n = 12345, r; const char * volatile f = "abc%1$n";
    if (!def_zero) TC_SKIP("count output already enabled");
    _set_printf_count_output(1);
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, &n);
    _set_printf_count_output(0);
    CHECK_EQ_I(r, 3);
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(b, "abc");
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    def_zero = (_get_printf_count_output() == 0);
    TC_RUN(positional_n_enabled);
    return tc_end();
}
