/* TC
 * id:    ucr_neg_stdio_vsscanf_n_conv_and_overflow_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.2 (out-of-range %d is UB); MS docs scanf (errno not documented for matching failure)
 * note:  PROBES: (1) "%d" of "99999999999" returns 0 or 1 without crashing (value not asserted), errno recorded not asserted;
 *        (2) a plain matching failure ("%d" of "abc") leaves errno unchanged (0).
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;
static int xs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vss(0ULL, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(overflow_no_crash)
{
    int a = 0, r; char b[16], c[8]; int e;
    errno = 0;
    r = xs("99999999999", "%d", &a);
    e = errno;
    CHECK(r == 0 || r == 1);
    tc_i64(r, b); tc_i64(e, c);
    tc_write("TC NOTE overflow ret="); tc_write(b); tc_write(" errno="); tc_write(c); tc_write("\n");
}
TC_CASE(errno_unchanged_on_match_fail)
{
    int a = 7, r;
    errno = 0;
    r = xs("abc", "%d", &a);
    CHECK_EQ_I(r, 0); CHECK_EQ_I(a, 7); CHECK_EQ_I(errno, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(overflow_no_crash); TC_RUN(errno_unchanged_on_match_fail);
    return tc_end();
}
