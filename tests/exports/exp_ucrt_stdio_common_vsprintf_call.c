/* TC
 * id:    exp_ucrt_stdio_common_vsprintf_call
 * area:  exports
 * funcs: __stdio_common_vsprintf
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * conf:  observed
 * ref:   stdio.h inline vsprintf wrappers; __stdio_common_vsprintf
 * note:  options=0 (no legacy flags). Called directly through a variadic wrapper.
 */
#include "tc.h"

#include <stdio.h>
#include <stdarg.h>

static int wrap(char *buf, size_t n, const char *fmt, ...)
{
    int r; va_list ap;
    va_start(ap, fmt);
    r = __stdio_common_vsprintf(0, buf, n, fmt, NULL, ap);
    va_end(ap);
    return r;
}
static const char *volatile v_fmt_d = "%d", *volatile v_fmt_s = "[%s]", *volatile v_str = "hello", *volatile v_fmt_long = "abc%dxyz";

TC_CASE(int_42) { char b[32]; int r = wrap(b, sizeof b, v_fmt_d, 42); CHECK_EQ_I(r, 2); CHECK_STR(b, "42"); }
TC_CASE(string_arg) { char b[32]; int r = wrap(b, sizeof b, v_fmt_s, v_str); CHECK_EQ_I(r, 7); CHECK_STR(b, "[hello]"); }
TC_CASE(count_return_value) { char b[32]; int r = wrap(b, sizeof b, v_fmt_long, 12345); CHECK_EQ_I(r, 11); CHECK_STR(b, "abc12345xyz"); }
int main(void) { tc_begin(); TC_RUN(int_42); TC_RUN(string_arg); TC_RUN(count_return_value); return tc_end(); }
