/* TC
 * id:    pfo_vsnprintf_exact_nonul_probe
 * area:  stdio_printf
 * funcs: _vsnprintf
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _snprintf: "If len == count, ... no null terminator is appended"
 * note:  PROBES whether _vsnprintf leaves byte [count] untouched (no NUL) when output length == count.
 *        Docs state this for _snprintf; the UCRT legacy-null-termination option makes it worth verifying on real Windows.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vs(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap);
    return r;
}
TC_CASE(no_nul_at_exact)
{
    char b[16]; volatile size_t n = 4; int r, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    r = vs(b, n, "%s", "wxyz");
    CHECK_EQ_I(r, 4);
    CHECK_MEM(b, "wxyz", 4);
    CHECK_EQ_I(b[4], 'Z');
}
int main(void) { tc_begin(); TC_RUN(no_nul_at_exact); return tc_end(); }
