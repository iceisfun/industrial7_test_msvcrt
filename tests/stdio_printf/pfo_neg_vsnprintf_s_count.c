/* TC
 * id:    pfo_neg_vsnprintf_s_count
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s ("if count < the needed length, -1 is returned and the buffer holds count chars + NUL; if buffer size is too small for count and not _TRUNCATE, invalid parameter handler, buffer[0]=0, -1")
 * note:  errno: ERANGE for too-small buffer, EINVAL for NULL args (_snprintf_s docs).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int cn(char *b, size_t n, size_t c, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, c, f, NULL, ap); va_end(ap); return r; }
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(count_limits)
{   /* count < len: -1, count chars + NUL (buffer large enough) */
    char b[16]; volatile size_t c = 3; int r; fill(b); tc_iph_count = 0;
    r = cn(b, 16, c, "%s", "abcdef");
    CHECK_EQ_I(r, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(buffer_too_small)
{
    char b[16]; volatile size_t n = 4; int r; fill(b); tc_iph_count = 0; errno = 0;
    r = cn(b, n, 10, "%s", "abcdef");   /* not _TRUNCATE and buffer cannot hold output */
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(errno, ERANGE);
}
TC_CASE(null_buffer)
{
    int r; tc_iph_count = 0; errno = 0;
    r = cn(NULL, 16, 5, "%d", 1);
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(null_format)
{
    char b[16]; int r; fill(b); tc_iph_count = 0; errno = 0;
    r = cn(b, 16, 5, NULL);
    CHECK(r < 0); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(count_limits); TC_RUN(buffer_too_small); TC_RUN(null_buffer); TC_RUN(null_format);
    return tc_end();
}
