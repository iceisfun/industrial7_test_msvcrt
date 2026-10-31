/* TC
 * id:    pfo_vsprintf_options_unknown_bits
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   undocumented
 * note:  PROBE: undefined/high option bits are ignored: output and return unchanged. Bit 0x80, bit 40, and all-ones (which also sets 0x1,0x2,0x8,0x10 etc.; for a plain %d those are no-ops).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static int vw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

static char b[32];
TC_CASE(bit40)  { int n = vs(1ULL << 40, b, sizeof b, "%d", (int)vi); CHECK_EQ_I(n, 2); CHECK_STR(b, "42"); }
TC_CASE(bit80)  { int n = vs(0x80, b, sizeof b, "%d", (int)vi); CHECK_EQ_I(n, 2); CHECK_STR(b, "42"); }
TC_CASE(allones){ int n = vs(~0ULL, b, sizeof b, "%d", (int)vi); CHECK_EQ_I(n, 2); CHECK_STR(b, "42"); }
int main(void)
{
    tc_begin();
    TC_RUN(bit40); TC_RUN(bit80); TC_RUN(allones);
    return tc_end();
}
