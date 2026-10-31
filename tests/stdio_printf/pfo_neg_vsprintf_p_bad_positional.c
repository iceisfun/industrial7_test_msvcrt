/* TC
 * id:    pfo_neg_vsprintf_p_bad_positional
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf_p positional parameters; sprintf_p, _sprintf_p_l: NULL buffer/format -> invalid parameter handler, returns -1
 * note:  Position zero, mixing, gaps and > _ARGMAX positions are in pfo_neg_vsprintf_p_positional_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cp(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsprintf_p(0, b, n, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(valid_reference)
{
    char b[32]; int r;
    tc_iph_count = 0;
    r = cp(b, sizeof b, "%2$s %1$d", 7, "x");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "x 7");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(null_buffer)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cp(b, 16, "%1$d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[16]; const char * volatile f = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cp(b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(valid_reference); TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}
