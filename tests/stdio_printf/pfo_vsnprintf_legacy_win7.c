/* TC
 * id:    pfo_vsnprintf_legacy_win7
 * area:  stdio_printf
 * funcs: _vsnprintf
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snprintf_l, _snwprintf: "If len < count, the string is terminated; if len == count, count characters are written and len is returned, no terminating null; if len > count, count characters are written and a negative value is returned"
 * note:  The "no NUL appended when len == count" detail is checked in pfo_vsnprintf_exact_nonul_probe, not here.
 *        Bytes beyond count must never be written.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define G 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = G; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }

static int vs(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsnprintf(b, n, f, ap); va_end(ap);
    return r;
}

TC_CASE(fits)
{
    char b[32]; volatile size_t n = 16; int r;
    fill(b, 32);
    r = vs(b, n, "%s-%d", "abc", 42);
    CHECK_EQ_I(r, 6);
    CHECK_STR(b, "abc-42");
    CHECK(guard_ok(b, 16, 32));
}
TC_CASE(count_less_than_len)
{
    char b[32]; volatile size_t n = 4; int r;
    fill(b, 32);
    r = vs(b, n, "%s", "abcdefgh");
    CHECK(r < 0);   /* docs: negative value */
    CHECK_MEM(b, "abcd", 4);
    CHECK(guard_ok(b, 4, 32));
}
TC_CASE(count_equals_len)
{
    char b[32]; volatile size_t n = 8; int r;
    fill(b, 32);
    r = vs(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_MEM(b, "abcdefgh", 8);
    CHECK(guard_ok(b, 8, 32));
}
TC_CASE(count_one_more_than_len)
{
    char b[32]; volatile size_t n = 9; int r;
    fill(b, 32);
    r = vs(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, 8);
    CHECK_STR(b, "abcdefgh");
    CHECK(guard_ok(b, 9, 32));
}
TC_CASE(count_zero)
{
    char b[8]; volatile size_t n = 0; int r;
    fill(b, 8);
    r = vs(b, n, "%s", "abc");
    CHECK(r < 0);   /* docs: negative value */
    CHECK(guard_ok(b, 0, 8));
}

int main(void)
{
    tc_begin();
    TC_RUN(fits); TC_RUN(count_less_than_len); TC_RUN(count_equals_len);
    TC_RUN(count_one_more_than_len); TC_RUN(count_zero);
    return tc_end();
}
