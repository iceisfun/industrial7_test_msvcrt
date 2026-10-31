/* TC
 * id:    pfo_neg_vsnprintf_s_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsnprintf_s
 * procs: _set_invalid_parameter_handler
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _vsnprintf_s: count < len -> truncated, NUL-terminated, returns -1, no handler; _TRUNCATE -> truncate, returns -1; buffer too small without _TRUNCATE -> handler, buffer[0]=0, returns -1
 * note:  errno of the failing paths is not asserted except non-zero for the handler path. NULL buffer cases are in pfo_neg_vsnprintf_s_null_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

#define G 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = G; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != G) return 0; return 1; }

static int cv(char *b, size_t n, size_t cnt, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = __stdio_common_vsnprintf_s(0, b, n, cnt, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(overflow_no_truncate)
{
    char b[16]; volatile size_t n = 4, c = 10; int r;
    fill(b, 16); tc_iph_count = 0; errno = 0;
    r = cv(b, n, c, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK(errno != 0);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK(guard_ok(b, 4, 16));
}
TC_CASE(count_truncates)
{
    char b[16]; volatile size_t n = 8, c = 3; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(b, "abc");
    CHECK(guard_ok(b, 8, 16));
}
TC_CASE(truncate_flag)
{
    char b[16]; volatile size_t n = 4, c = (size_t)-1; int r;   /* _TRUNCATE */
    fill(b, 16); tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK_STR(b, "abc");
    CHECK(guard_ok(b, 4, 16));
}
TC_CASE(truncate_flag_fits)
{
    char b[16]; volatile size_t n = 16, c = (size_t)-1; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(count_ge_size_but_text_fits)
{
    char b[16]; volatile size_t n = 6, c = 10; int r;
    fill(b, 16); tc_iph_count = 0;
    r = cv(b, n, c, "%s", "abc");
    CHECK_EQ_I(r, 3);
    CHECK_STR(b, "abc");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(overflow_no_truncate); TC_RUN(count_truncates); TC_RUN(truncate_flag);
    TC_RUN(truncate_flag_fits); TC_RUN(count_ge_size_but_text_fits);
    return tc_end();
}
