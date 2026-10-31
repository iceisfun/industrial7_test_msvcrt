/* TC
 * id:    pfo_neg_vsprintf_s_overflow
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s ("if buffer too small: buffer[0]=0, invalid parameter handler invoked, returns -1")
 * note:  errno: ERANGE for too-small buffer, EINVAL for NULL args (sprintf_s docs). Calls the common function directly with options 0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int calls(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(too_small)
{
    char b[16]; volatile size_t n = 4; int r; fill(b); tc_iph_count = 0; errno = 0;
    r = calls(0, b, n, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(one_short)
{
    char b[16]; volatile size_t n = 6; int r; fill(b); tc_iph_count = 0; errno = 0;
    r = calls(0, b, n, "%s", "abcdef");   /* needs 7 */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[6], 'Z'); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(null_format)
{
    char b[16]; int r; fill(b); tc_iph_count = 0; errno = 0;
    r = calls(0, b, 16, NULL);
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_buffer)
{
    int r; tc_iph_count = 0; errno = 0;
    r = calls(0, NULL, 16, "%d", 1);
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(too_small); TC_RUN(one_short); TC_RUN(null_format); TC_RUN(null_buffer);
    return tc_end();
}
