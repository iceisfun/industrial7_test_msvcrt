/* TC
 * id:    pfo_neg_vsprintf_p_win7
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vsprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _sprintf_p, _sprintf_p_l, _vsprintf_p: NULL buffer/format or too-small buffer -> invalid parameter handler, returns -1
 * note:  Mixing of positional/non-positional is in pfo_neg_vsprintf_p_positional_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define G 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = G; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }

static int cp(char *b, size_t n, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = _vsprintf_p(b, n, f, ap); va_end(ap);
    return r;
}
TC_CASE(null_buffer)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0;
    r = cp(b, 16, "%1$d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[16]; const char * volatile f = NULL; int r;
    tc_iph_count = 0;
    r = cp(b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(too_small)
{
    char b[16]; volatile size_t n = 4; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cp(b, n, "%1$s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(guard_ok(b, 4, 16));
}
TC_CASE(fits)
{
    char b[16]; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cp(b, sizeof b, "%2$s-%1$d", 5, "ab");
    CHECK_EQ_I(r, 4);
    CHECK_STR(b, "ab-5");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(too_small); TC_RUN(fits);
    return tc_end();
}
