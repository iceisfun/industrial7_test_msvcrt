/* TC
 * id:    pfs_neg_probe_sprintf_s_overflow_errno
 * area:  stdio_printf
 * funcs:
 * procs: sprintf_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s (errno on buffer overflow not documented)
 * note:  PROBE: errno == ERANGE after sprintf_s overflows its buffer (handler returns).
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#undef sprintf_s
TC_DECL_PROC(int, sprintf_s, (char *, size_t, const char *, ...));
#define sprintf_s tc_p_sprintf_s
static const char *volatile s6 = "abcdef";
TC_CASE(erange) {
    char b[4]; int n;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    n = sprintf_s(b, sizeof b, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, ERANGE);
}
int main(void) { tc_begin(); TC_LOAD_PROC(sprintf_s); if (!tc_p_sprintf_s) { tc_write("TC FAIL no sprintf_s\n"); return 1; } TC_RUN(erange); return tc_end(); }
