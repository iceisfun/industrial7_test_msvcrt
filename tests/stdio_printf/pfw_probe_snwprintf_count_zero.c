/* TC
 * id:    pfw_probe_snwprintf_count_zero
 * area:  stdio_printf
 * funcs: _snwprintf
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _snwprintf
 * note:  PROBE: _snwprintf with count==0. Non-NULL buffer: assert ret==-1 and buffer untouched (likely). NULL buffer: return value and iph activity only recorded (TC NOTE lines), not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static void note(const char *k, long long v) { char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }

TC_CASE(null_buf_count0)
{
    wchar_t *volatile p = NULL; volatile int v = 12; int n;
    if (!tc_iph_install()) TC_SKIP("no handler api");
    tc_iph_count = 0; errno = 0;
    n = _snwprintf(p, 0, L"%d", v);
    note("ret", n); note("iph", tc_iph_count); note("errno", errno);
    CHECK(n == -1 || n == 2);
}
TC_CASE(nonnull_buf_count0_buffer_untouched_ret_m1)
{
    wchar_t b[8]; volatile int v = 12; int n, i;
    for (i = 0; i < 8; i++) b[i] = L'Z';
    n = _snwprintf(b, 0, L"%d", v);
    CHECK_EQ_I(n, -1);
    for (i = 0; i < 8; i++) CHECK(b[i] == L'Z');
}
int main(void) { tc_begin(); TC_RUN(null_buf_count0); TC_RUN(nonnull_buf_count0_buffer_untouched_ret_m1); return tc_end(); }
