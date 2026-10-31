/* TC
 * id:    ucr_stdio_printf_count_output_toggle
 * area:  ucrt_only
 * funcs: _set_printf_count_output _get_printf_count_output
 * procs: __stdio_common_vsprintf
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _set_printf_count_output, _get_printf_count_output
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vs_t p_vs;
static int calls(char *b, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vs(0, b, 64, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(toggle)
{
    char b[64]; volatile int target = -1; int r;
    if (!p_vs) TC_SKIP("not exported");
    CHECK_EQ_I(_get_printf_count_output(), 0);
    CHECK_EQ_I(_set_printf_count_output(1), 0);
    CHECK_EQ_I(_get_printf_count_output(), 1);
    r = calls(b, "abcd%nef", (int *)&target);
    CHECK_STR(b, "abcdef"); CHECK_EQ_I(r, 6); CHECK_EQ_I(target, 4);
    CHECK_EQ_I(_set_printf_count_output(0), 1);
    CHECK_EQ_I(_get_printf_count_output(), 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    TC_RUN(toggle);
    return tc_end();
}
