/* TC
 * id:    pff_neg_sprintf_s_f_huge_value
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s snprintf _snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s (overflow -> handler, empty string, -1), _snprintf (count exceeded -> -1, no NUL), snprintf (C99: full length, always NUL-terminated); C11 7.21.6.5
 * note:  1e308 printed with %f is 309 integer digits + '.' + 6 = 316 characters. Canary bytes guard against overruns.
 *        errno and exact digits are in pff_neg_probe_erange_float_overflow.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

static void fill(char *b, size_t n) { size_t i; for (i = 0; i < n; i++) b[i] = (char)0x5a; }
static int canary_ok(const char *b, size_t from, size_t to) { size_t i; for (i = from; i < to; i++) if (b[i] != (char)0x5a) return 0; return 1; }

TC_CASE(sprintf_s_overflow)
{
    char b[400]; volatile double v = 1e308; int r;
    fill(b, sizeof b); tc_iph_count = 0;
    r = sprintf_s(b, 64, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
    CHECK(canary_ok(b, 64, sizeof b));
}
TC_CASE(legacy_snprintf_no_overrun)
{
    char b[400]; volatile double v = 1e308; int r;
    fill(b, sizeof b); tc_iph_count = 0;
    r = _snprintf(b, 63, "%f", v);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 0);
    CHECK(canary_ok(b, 63, sizeof b));   /* count==63 bytes at most are written */
    CHECK(b[0] == '1');
}
TC_CASE(c99_snprintf_full_length)
{
    char b[400]; volatile double v = 1e308; int r;
    fill(b, sizeof b);
    r = snprintf(b, 64, "%f", v);
    CHECK_EQ_I(r, 316);
    CHECK_EQ_I(b[63], 0);
    CHECK(canary_ok(b, 64, sizeof b));
    CHECK_EQ_I(strlen(b), 63);
    CHECK(b[0] == '1');
}
TC_CASE(c99_length_query_prec512)
{
    volatile double v = 1e308; int r;
    r = snprintf(NULL, 0, "%.512f", v);
    CHECK_EQ_I(r, 309 + 1 + 512);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_s_overflow); TC_RUN(legacy_snprintf_no_overrun); TC_RUN(c99_snprintf_full_length); TC_RUN(c99_length_query_prec512);
    return tc_end();
}
