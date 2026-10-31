/* TC
 * id:    pfo_neg_vsprintf_legacy_trunc_returns_minus1
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _vsnprintf (if the formatted length exceeds count, count bytes are written and -1 is returned)
 * note:  Legacy (options 0) truncation: no NUL is guaranteed, so only the return and the bytes up to count are checked.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int c0(unsigned o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(legacy_trunc_returns_minus1)
{
    char b[16]; int r; int i;
    for (i = 0; i < 16; i++) b[i] = (char)0xA5;
    r = c0(0, b, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_MEM(b, "abcd", 4); CHECK_EQ_I((unsigned char)b[4], 0xA5);
}
int main(void)
{
    tc_begin();
    TC_RUN(legacy_trunc_returns_minus1);
    return tc_end();
}
