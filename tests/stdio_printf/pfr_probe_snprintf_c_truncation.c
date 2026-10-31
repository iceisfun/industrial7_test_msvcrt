/* TC
 * id:    pfr_probe_snprintf_c_truncation
 * area:  stdio_printf
 * funcs:
 * procs: _snprintf_c
 * crt:   win7
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf, _snprintf_c, _snprintf_c_l (c variants)
 * note:  PROBE: _snprintf_c, unlike _snprintf, always NUL-terminates; on truncation it returns -1 and b[count-1]==0.
 *        _snprintf_c is not exported by ucrtbase (inline there), hence win7 only.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(int, _snprintf_c, (char *, size_t, const char *, ...));

TC_CASE(truncated)
{
    char b[16]; const char * volatile f = "abcdef"; int r, i;
    for (i = 0; i < 16; i++) b[i] = 'Z';
    if (!tc_p__snprintf_c) TC_SKIP("not exported");
    r = tc_p__snprintf_c(b, 4, f);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(b[3], 0);
    CHECK(b[0] == 'a' && b[1] == 'b' && b[2] == 'c');
    CHECK_EQ_I(b[4], 'Z');
}
int main(void) { tc_begin(); TC_LOAD_PROC(_snprintf_c); TC_RUN(truncated); return tc_end(); }
