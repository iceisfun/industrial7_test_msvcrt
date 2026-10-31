/* TC
 * id:    ucr_stdio_vsscanf_n_not_counted
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p12 (%n: no input consumed, does not increment the assignment count)
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;
static int xs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vss(0ULL, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(n_after_d)
{
    int a = 0, n = -1; int r = xs("12", "%d%n", &a, &n);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12); CHECK_EQ_I(n, 2);
}

int main(void)
{
    tc_begin();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(n_after_d);
    return tc_end();
}
