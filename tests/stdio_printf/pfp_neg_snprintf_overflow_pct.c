/* TC
 * id:    pfp_neg_snprintf_overflow_pct
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _snprintf: returns a negative value if more than count chars would be written; no terminating NUL if len == count
 * note:  uses a 16-char %s argument so results do not depend on %p formatting or pointer width.
 *        Guard bytes beyond count must stay untouched.
 */
#include "tc.h"
#include <stdio.h>

#define GUARD 0x5A
static void fill(char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = GUARD; }
static int guard_ok(const char *b, int from, int to) { int i; for (i = from; i < to; i++) if ((unsigned char)b[i] != GUARD) return 0; return 1; }
static const char *volatile vp = "0123456789ABCDEF";

TC_CASE(count_4_truncates)
{
    char b[32]; int r;
    fill(b, 32);
    r = _snprintf(b, 4, "%s", vp);
    CHECK_EQ_I(r, -1);
    CHECK_MEM(b, "0123", 4);
    CHECK(guard_ok(b, 4, 32));
}
TC_CASE(count_16_no_nul)
{
    char b[32]; int r;
    fill(b, 32);
    r = _snprintf(b, 16, "%s", vp);
    CHECK_EQ_I(r, 16);
    CHECK_MEM(b, "0123456789ABCDEF", 16);
    CHECK_EQ_I((unsigned char)b[16], GUARD);   /* no NUL written */
    CHECK(guard_ok(b, 16, 32));
}
TC_CASE(count_17_terminated)
{
    char b[32]; int r;
    fill(b, 32);
    r = _snprintf(b, 17, "%s", vp);
    CHECK_EQ_I(r, 16);
    CHECK_STR(b, "0123456789ABCDEF");
    CHECK(guard_ok(b, 17, 32));
}
TC_CASE(pct_pct_truncate)
{
    char b[32]; int r;
    fill(b, 32);
    r = _snprintf(b, 2, "%%%%%%");   /* "%%%": three chars into 2 */
    CHECK_EQ_I(r, -1);
    CHECK_MEM(b, "%%", 2);
    CHECK(guard_ok(b, 2, 32));
}
int main(void)
{
    tc_begin();
    TC_RUN(count_4_truncates); TC_RUN(count_16_no_nul); TC_RUN(count_17_terminated); TC_RUN(pct_pct_truncate);
    return tc_end();
}
