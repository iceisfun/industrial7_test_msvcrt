/* TC
 * id:    pfw_neg_snwprintf_no_overrun_guard
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snwprintf: at most count wide characters are written
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

#define CAN L'#'
struct buf { wchar_t pre[8]; wchar_t b[8]; wchar_t post[200]; };
static struct buf g;
static wchar_t big[3000];

static void init(void)
{
    int i;
    for (i = 0; i < 8; i++) { g.pre[i] = CAN; g.b[i] = L'.'; }
    for (i = 0; i < 200; i++) g.post[i] = CAN;
}
static int post_ok(void) { int i; for (i = 0; i < 200; i++) if (g.post[i] != CAN) return 0; return 1; }
static int pre_ok(void) { int i; for (i = 0; i < 8; i++) if (g.pre[i] != CAN) return 0; return 1; }

TC_CASE(long_string)
{
    const wchar_t *volatile s = L"0123456789012345678901234567890123456789"; int n;
    init(); n = _snwprintf(g.b, 8, L"%ls", s);
    CHECK_EQ_I(n, -1); CHECK(post_ok()); CHECK(pre_ok());
}
TC_CASE(huge_ls_arg)
{
    int i, n;
    for (i = 0; i < 2999; i++) big[i] = L'q';
    big[2999] = 0;
    init(); n = _snwprintf(g.b, 8, L"%ls", (const wchar_t *volatile)big);
    CHECK_EQ_I(n, -1); CHECK(post_ok()); CHECK(pre_ok());
    CHECK(g.b[7] == L'q');
}
TC_CASE(width_100)
{
    volatile int v = 5; int n;
    init(); n = _snwprintf(g.b, 8, L"%100d", v);
    CHECK_EQ_I(n, -1); CHECK(post_ok()); CHECK(pre_ok());
    CHECK(g.b[0] == L' ' && g.b[7] == L' ');
}
TC_CASE(canary_before_buffer)
{
    volatile int v = 123456789; int n;
    init(); n = _snwprintf(g.b, 3, L"%d", v);
    CHECK_EQ_I(n, -1); CHECK(pre_ok()); CHECK(g.b[3] == L'.'); CHECK(post_ok());
}
int main(void) { tc_begin(); TC_RUN(long_string); TC_RUN(huge_ls_arg); TC_RUN(width_100); TC_RUN(canary_before_buffer); return tc_end(); }
