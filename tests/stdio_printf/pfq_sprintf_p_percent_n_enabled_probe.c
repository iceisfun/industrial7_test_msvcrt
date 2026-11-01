/* TC
 * id:    pfq_sprintf_p_percent_n_enabled_probe
 * area:  stdio_printf
 * kind:  positive
 * funcs: _vsprintf_p _set_printf_count_output _get_printf_count_output
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   MS docs _set_printf_count_output (enables %n); positional %n in _vsprintf_p is not documented separately
 * note:  Observed on Windows 10 22H2: even after _set_printf_count_output(1) (getter returns 1), the _vsprintf_p family rejects %n, positional
 *        or not (reference probe: plain "abc%n" through __stdio_common_vsprintf_p is also rejected): returns -1, errno EINVAL, one handler call,
 *        and the target int is left unchanged.
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
    CHECK_EQ_I(_get_printf_count_output(), 1);
    tc_iph_count = 0;
    r = cp(b, sizeof b, f, &n);
    _set_printf_count_output(0);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(n, 12345);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    def_zero = (_get_printf_count_output() == 0);
    TC_RUN(positional_n_enabled);
    return tc_end();
}
