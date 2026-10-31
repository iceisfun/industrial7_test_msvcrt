/* TC
 * id:    pfs_neg_probe_percent_c_pad_zero_trunc
 * area:  stdio_printf
 * funcs: _snprintf
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field characters (%c with 0), sprintf_s, _snprintf
 * note:  PROBE: whether a %c argument of 0 is emitted as an embedded NUL byte that counts toward the
 *        return value and toward the buffer size, in _snprintf and sprintf_s (incl. the overflow case).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));

TC_CASE(snprintf_nul_counted)
{
    char b[8]; volatile int z = 0; int n;
    memset(b, 'Z', sizeof b);
    n = _snprintf(b, 3, "%c%c%c", 'a', z, 'b');
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 'b');
    CHECK_EQ_I(b[3], 'Z');
}
TC_CASE(sprintf_s_fits)
{
    char b[8]; volatile int z = 0; int n;
    if (!tc_p_sprintf_s) TC_SKIP("sprintf_s not exported");
    memset(b, 'Z', sizeof b);
    n = tc_p_sprintf_s(b, 3, "%c%c", 'a', z);
    CHECK_EQ_I(n, 2);
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 0); CHECK_EQ_I(b[2], 0);
}
TC_CASE(sprintf_s_overflow)
{
    char b[8]; volatile int z = 0; int n;
    if (!tc_p_sprintf_s) TC_SKIP("sprintf_s not exported");
    if (!tc_iph_install()) TC_SKIP("no iph");
    memset(b, 'Z', sizeof b); tc_iph_count = 0;
    n = tc_p_sprintf_s(b, 2, "%c%c", 'a', z);
    CHECK_EQ_I(n, -1);
    CHECK_EQ_I(tc_iph_count, 1);
    CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(sprintf_s);
    TC_RUN(snprintf_nul_counted); TC_RUN(sprintf_s_fits); TC_RUN(sprintf_s_overflow);
    return tc_end();
}
