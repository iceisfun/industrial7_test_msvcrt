/* TC
 * id:    ucr_neg_stdio_vsscanf_securecrt_small
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   MS docs scanf_s / Parameter validation (secure CRT too-small buffer is an error)
 * note:  PROBES that a too-small scanf_s destination yields return count excluding the failed item and buffer[0]==0 (exact wording not verified against docs; assertion is from memory). Secure-CRT scanf with a destination too small for the input. A handler is installed so the invalid-parameter path returns.
 */
#include "tc.h"
#include <stdarg.h>
#include <string.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;

static int xs(const char *in, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt);
    r = p_vss(1ULL, in, (size_t)-1, fmt, NULL, ap);
    va_end(ap);
    return r;
}

TC_CASE(too_small_returns_zero)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b);
    r = xs("abcdef", "%s", b, 3u);
    CHECK_EQ_I(r, 0);
}
TC_CASE(too_small_buf0_cleared)
{
    char b[8]; memset(b, 'z', sizeof b);
    xs("abcdef", "%s", b, 3u);
    CHECK(b[0] == 0);
}
TC_CASE(earlier_assignment_counted)
{
    int n = 0; char b[8]; int r;
    memset(b, 'z', sizeof b);
    r = xs("7 abcdef", "%d %s", &n, b, 3u);
    CHECK_EQ_I(n, 7); CHECK_EQ_I(r, 1); CHECK(b[0] == 0);
}
TC_CASE(nul_no_room_exact)
{
    char b[8]; int r;   /* "abc" needs 4 with the NUL, size 3 is too small */
    memset(b, 'z', sizeof b);
    r = xs("abc", "%s", b, 3u);
    CHECK_EQ_I(r, 0); CHECK(b[0] == 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(too_small_returns_zero); TC_RUN(too_small_buf0_cleared); TC_RUN(earlier_assignment_counted); TC_RUN(nul_no_room_exact);
    return tc_end();
}
