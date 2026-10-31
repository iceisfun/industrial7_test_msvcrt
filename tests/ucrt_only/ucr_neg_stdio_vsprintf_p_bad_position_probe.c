/* TC
 * id:    ucr_neg_stdio_vsprintf_p_bad_position_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf_p
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf_p positional parameters
 * note:  PROBE: invalid positional forms ('%0$d', mixing positional with plain, gap in positions) are expected to return -1 with
 *        EINVAL and one invalid-parameter call. The docs do not spell this out per form.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vsp_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vsp_t p_vsp;
static int call(char *b, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = p_vsp(0, b, 64, f, NULL, ap); va_end(ap); return r; }

TC_CASE(position_zero)
{
    char b[64]; int r;
    if (!p_vsp) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = call(b, "%0$d", 1, 2, 3);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(mixed_plain)
{
    char b[64]; int r;
    if (!p_vsp) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = call(b, "%1$d %d", 1, 2, 3);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(gap)
{
    char b[64]; int r;
    if (!p_vsp) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = call(b, "%1$d %3$d", 1, 2, 3);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsp = (vsp_t)tc_crt_proc("__stdio_common_vsprintf_p");
    TC_RUN(position_zero); TC_RUN(mixed_plain); TC_RUN(gap);
    return tc_end();
}
