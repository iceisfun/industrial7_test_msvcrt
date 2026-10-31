/* TC
 * id:    ucr_stdio_vsscanf_width_split
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   win10
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.2p9 (maximum field width); MS docs scanf width specification
 */
#include "tc.h"
#include <stdarg.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static vsscanf_t p_vss;

static int xs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vss(0ULL, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(width3_then_rest)
{
    int a = 0, b = 0; int r = xs("12345", "%3d%d", &a, &b);
    CHECK_EQ_I(r, 2); CHECK_EQ_I(a, 123); CHECK_EQ_I(b, 45);
}
TC_CASE(width_stops_at_nondigit)
{
    int a = 0; int r = xs("12x", "%5d", &a);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(a, 12);
}

int main(void)
{
    tc_begin();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    if (!p_vss) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(width3_then_rest); TC_RUN(width_stops_at_nondigit);
    return tc_end();
}
