/* TC
 * id:    pfo_vsprintf_opt_combo_01_02
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   stdio.h inline _vsnprintf/snprintf option masks; C11 7.21.6.5
 * note:  PROBE: the combination 0x1|0x2 is never used by the shipped headers (snprintf=2, _snprintf=1), so its behaviour is undocumented. Expected: standard snprintf semantic (return would-be length, always NUL-terminated).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(trunc)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(3, b, 4, "abcdefgh");
    CHECK_EQ_I(n, 8); CHECK_EQ_I(b[2], 'c'); CHECK_EQ_I(b[3], 0); CHECK_EQ_I(b[4], 'Z');
}
TC_CASE(size_query)
{
    int n = vs(3, NULL, 0, "abcdefgh%d", 12);
    CHECK_EQ_I(n, 10);
}
TC_CASE(fits)
{
    char b[16]; int n;
    n = vs(3, b, sizeof b, "%s", "hi");
    CHECK_EQ_I(n, 2); CHECK_STR(b, "hi");
}
int main(void)
{
    tc_begin();
    TC_RUN(trunc); TC_RUN(size_query); TC_RUN(fits);
    return tc_end();
}
