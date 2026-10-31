/* TC
 * id:    pfo_neg_vsprintf_options0_trunc
 * area:  stdio_printf
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   options=0 is the _vsprintf_l-style entry (no public snprintf API); analogous to MS docs _snprintf / _vsnprintf return value (len > count: count chars stored, negative return)
 * note:  options=0 truncation. Only the documented parts are asserted: negative return and the first count chars stored, bytes past count untouched. Whether a NUL is stored is left to pfo_probe_vsprintf_trunc_nul.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

static int vs(unsigned long long o, char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(o, b, n, f, NULL, ap); va_end(ap); return r; }
static volatile int vi = 42;

TC_CASE(trunc_returns_neg)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, 4, "abcdefgh");
    CHECK(n < 0);
}
TC_CASE(trunc_prefix_stored)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(0, b, 4, "abcdefgh");
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 'b'); CHECK_EQ_I(b[2], 'c');
}
TC_CASE(sentinel_untouched)
{
    char b[16];
    memset(b, 'Z', sizeof b);
    vs(0, b, 4, "abcdefgh");
    CHECK_EQ_I(b[4], 'Z'); CHECK_EQ_I(b[5], 'Z'); CHECK_EQ_I(b[15], 'Z');
}
TC_CASE(one_short_still_negative)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, 7, "abcdefgh");
    CHECK(n < 0); CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[2], 'c'); CHECK_EQ_I(b[8], 'Z');
}
TC_CASE(room_for_nul_returns_len)
{
    char b[16]; int n;
    memset(b, 'Z', sizeof b);
    n = vs(0, b, 9, "abcdefgh");
    CHECK_EQ_I(n, 8); CHECK_STR(b, "abcdefgh"); CHECK_EQ_I(b[9], 'Z');
}
int main(void)
{
    tc_begin();
    TC_RUN(trunc_returns_neg); TC_RUN(trunc_prefix_stored); TC_RUN(sentinel_untouched);
    TC_RUN(one_short_still_negative); TC_RUN(room_for_nul_returns_len);
    return tc_end();
}
