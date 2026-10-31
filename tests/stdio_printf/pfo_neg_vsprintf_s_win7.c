/* TC
 * id:    pfo_neg_vsprintf_s_win7
 * area:  stdio_printf
 * kind:  negative
 * funcs: vsprintf_s
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * conf:  documented
 * ref:   MS docs vsprintf_s, _vsprintf_s_l: invalid parameter handler invoked for NULL buffer/format, zero size, too-small buffer; returns -1
 * note:  msvcr80 and msvcrt are excluded (msvcr80 import-library gap; msvcrt.dll has no vsprintf_s).
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
    va_start(ap, f); r = vsprintf_s(b, n, f, ap); va_end(ap);
    return r;
}
TC_CASE(too_small)
{
    char b[16]; volatile size_t n = 4; int r;
    fill(b, 16); tc_iph_count = 0;
    r = vs(b, n, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK(guard_ok(b, 4, 16));
}
TC_CASE(null_buffer)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0;
    r = vs(b, 8, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_format)
{
    char b[8]; const char * volatile f = NULL; int r;
    fill(b, 8); tc_iph_count = 0;
    r = vs(b, sizeof b, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(size_zero)
{
    char b[8]; volatile size_t n = 0; int r;
    fill(b, 8); tc_iph_count = 0;
    r = vs(b, n, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(guard_ok(b, 0, 8));
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(too_small); TC_RUN(null_buffer); TC_RUN(null_format); TC_RUN(size_zero);
    return tc_end();
}
