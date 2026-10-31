/* TC
 * id:    pfq_neg_sprintf_p_percent_n
 * area:  stdio_printf
 * kind:  negative
 * funcs: _set_printf_count_output _get_printf_count_output
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_printf_count_output / _get_printf_count_output (%n support is off by default since VS2005; setter returns previous setting)
 * note:  Only the documented global setting is asserted here (default off, setter returns previous value). What _vsprintf_p does with
 *        %n (plain or positional) is probed separately in pfq_neg_sprintf_p_percent_n_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int def_zero;

TC_CASE(default_off)
{
    def_zero = (_get_printf_count_output() == 0);
    CHECK(def_zero);
}
TC_CASE(set_returns_previous)
{
    int p;
    if (!def_zero) TC_SKIP("count output already enabled");
    p = _set_printf_count_output(1);
    CHECK_EQ_I(p, 0);
    CHECK_EQ_I(_get_printf_count_output(), 1);
    p = _set_printf_count_output(0);
    CHECK_EQ_I(p, 1);
    CHECK_EQ_I(_get_printf_count_output(), 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(default_off); TC_RUN(set_returns_previous);
    return tc_end();
}
