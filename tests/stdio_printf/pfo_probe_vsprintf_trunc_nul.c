/* TC
 * id:    pfo_probe_vsprintf_trunc_nul
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _snprintf remarks
 * note:  PROBE: options=0 NUL-termination state on truncation and on exact fit (count == len). Docs for _snprintf say no NUL is appended when len >= count and that exact fit returns len; not certain for the UCRT entry point itself.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(exact_fit_return)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, 8, "abcdefgh");
    CHECK_EQ_I(n, 8);
}
TC_CASE(exact_fit_no_nul)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(0, b, 8, "abcdefgh");
    CHECK_EQ_I(b[7], 'h'); CHECK_EQ_I(b[8], 'Z');
}
TC_CASE(trunc_last_byte)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(0, b, 4, "abcdefgh");
    CHECK_EQ_I(b[3], 'd');
}
int main(void)
{
    tc_begin();
    TC_RUN(exact_fit_return); TC_RUN(exact_fit_no_nul); TC_RUN(trunc_last_byte);
    return tc_end();
}
