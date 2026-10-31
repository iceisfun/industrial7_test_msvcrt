/* TC
 * id:    pfo_probe_vsprintf_s_options
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   corecrt_stdio_config.h option bits (undocumented for the _s entry point)
 * note:  PROBE: passing option bits 0x1 / 0x2 to __stdio_common_vsprintf_s (a normal fit) is assumed to have no
 *        effect on the result. Not documented by Microsoft.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int calls(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(options_ignored_nul)
{
    char b[16]; int r = calls(2, b, 16, "%s", "xyz"); CHECK_EQ_I(r, 3); CHECK_STR(b, "xyz");
    r = calls(1, b, 16, "%s", "xyz"); CHECK_EQ_I(r, 3); CHECK_STR(b, "xyz");
}
int main(void) { tc_begin(); TC_RUN(options_ignored_nul); return tc_end(); }
