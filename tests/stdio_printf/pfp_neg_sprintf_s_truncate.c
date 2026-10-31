/* TC
 * id:    pfp_neg_sprintf_s_truncate
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _sprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_p ("if the buffer is too small ... buffer[0] set to 0, invalid parameter handler invoked, returns -1")
 * note:  errno values are deliberately NOT asserted here (see pfp_neg_sprintf_s_errno_probe).
 *        Guard bytes after the stated buffer size must stay untouched.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

#define GUARD 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = GUARD; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != GUARD) return 0; return 1; }

TC_CASE(too_small)
{
    char b[32]; volatile int v = 12345; int r;
    fill(b, 32); tc_iph_count = 0;
    r = sprintf_s(b, 4, "%d", v);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK(guard_ok(b, 4, 32));
}
TC_CASE(exact_fit_ok)
{
    char b[32]; volatile int v = 12345; int r;
    fill(b, 32); tc_iph_count = 0;
    r = sprintf_s(b, 6, "%d", v);   /* 5 chars + NUL fits exactly */
    CHECK_EQ_I(r, 5);
    CHECK_STR(b, "12345");
    CHECK_EQ_I(tc_iph_count, 0);
    CHECK(guard_ok(b, 6, 32));
}
TC_CASE(one_short)
{
    char b[32]; volatile int v = 12345; int r;
    fill(b, 32); tc_iph_count = 0;
    r = sprintf_s(b, 5, "%d", v);   /* needs 6 */
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
    CHECK(guard_ok(b, 5, 32));
}
TC_CASE(zero_size)
{
    char b[8]; volatile size_t sz = 0; int r;
    fill(b, 8); tc_iph_count = 0;
    r = sprintf_s(b, sz, "%d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK(guard_ok(b, 0, 8));       /* nothing may be written when size is 0 */
}
TC_CASE(sprintf_p_too_small)
{
    char b[32]; volatile int v = 12345; int r;
    fill(b, 32); tc_iph_count = 0;
    r = _sprintf_p(b, 4, "%1$d", v);
    CHECK_EQ_I(r, -1);
    CHECK(guard_ok(b, 4, 32));
}
TC_CASE(sprintf_p_fits)
{
    char b[32]; volatile int v = 12345; int r;
    fill(b, 32);
    r = _sprintf_p(b, 6, "%1$d", v);
    CHECK_EQ_I(r, 5);
    CHECK_STR(b, "12345");
}

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no _set_invalid_parameter_handler\n"); return 1; }
    TC_RUN(too_small); TC_RUN(exact_fit_ok); TC_RUN(one_short); TC_RUN(zero_size);
    TC_RUN(sprintf_p_too_small); TC_RUN(sprintf_p_fits);
    return tc_end();
}
