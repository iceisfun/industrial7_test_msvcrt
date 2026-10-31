/* TC
 * id:    pfo_vscprintf_basic
 * area:  stdio_printf
 * funcs: _vscprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scprintf_l, _vscprintf: "returns the number of characters that would be generated, not counting the terminating null"
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vc(const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vscprintf(f, ap); va_end(ap);
    return r;
}

TC_CASE(plain)   { CHECK_EQ_I(vc("hello"), 5); }
TC_CASE(empty)   { volatile const char *f = ""; CHECK_EQ_I(vc((const char *)f), 0); }
TC_CASE(args)    { CHECK_EQ_I(vc("%d %s", -123, "abc"), 8); }
TC_CASE(width)   { CHECK_EQ_I(vc("[%10d][%-6s]", 5, "ab"), 20); }
TC_CASE(long_str)
{
    static char s[1001]; int i;
    for (i = 0; i < 1000; i++) s[i] = 'x';
    s[1000] = 0;
    CHECK_EQ_I(vc("%s", s), 1000);
    CHECK_EQ_I(vc("<%s>", s), 1002);
}
int main(void) { tc_begin(); TC_RUN(plain); TC_RUN(empty); TC_RUN(args); TC_RUN(width); TC_RUN(long_str); return tc_end(); }
