/* TC
 * id:    pfi_neg_vsnprintf_truncation
 * area:  stdio_printf
 * funcs: _vsnprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs vsnprintf, _vsnprintf: -1 if more than count chars needed; no NUL when len == count
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int vs(char *b, size_t n, const char *f, ...)
{
    int r; va_list ap; va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap); return r;
}
static void fill(char *b) { int i; for (i = 0; i < 16; i++) b[i] = 'Z'; }

TC_CASE(truncated)
{
    char b[16]; int n; fill(b);
    n = vs(b, 3, "%d", 12345);
    CHECK_EQ_I(n, -1);
    CHECK(b[0] == '1' && b[1] == '2' && b[2] == '3');
}
TC_CASE(count_equals_len)
{
    char b[16]; int n; fill(b);
    n = vs(b, 5, "%d", 12345);
    CHECK_EQ_I(n, 5);
    CHECK(b[0] == '1' && b[4] == '5');
    CHECK(b[5] == 'Z');  /* no NUL written */
}
TC_CASE(guard_bytes)
{
    char b[16]; int n, i; fill(b);
    n = vs(b, 4, "%x", 0xabcdef);
    CHECK_EQ_I(n, -1);
    CHECK(b[0] == 'a' && b[3] == 'd');
    for (i = 4; i < 16; i++) CHECK(b[i] == 'Z');
}
int main(void) { tc_begin(); TC_RUN(truncated); TC_RUN(count_equals_len); TC_RUN(guard_bytes); return tc_end(); }
