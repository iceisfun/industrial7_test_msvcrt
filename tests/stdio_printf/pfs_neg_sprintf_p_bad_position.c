/* TC
 * id:    pfs_neg_sprintf_p_bad_position
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf_p positional parameters
 * note:  PROBE: invalid positional format strings (skipped position, position 0, mixing positional with non-positional) are
 *        assumed to fail via the invalid-parameter path: return -1 and iph invoked.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char * volatile f_skip = "%2$s";
static const char * volatile f_zero = "%0$s";
static const char * volatile f_mix  = "%1$s %s";

TC_CASE(skipped_position)
{
    char b[16]; int r; CHECK(tc_iph_install()); tc_iph_count = 0;
    r = _sprintf_p(b, 16, f_skip, "a", "b");
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1);
}
TC_CASE(position_zero)
{
    char b[16]; int r; CHECK(tc_iph_install()); tc_iph_count = 0; errno = 0;
    r = _sprintf_p(b, 16, f_zero, "a", "b");
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}
TC_CASE(mixed_modes)
{
    char b[16]; int r; CHECK(tc_iph_install()); tc_iph_count = 0;
    r = _sprintf_p(b, 16, f_mix, "a", "b");
    CHECK_EQ_I(r, -1); CHECK(tc_iph_count >= 1);
}
int main(void) { tc_begin(); TC_RUN(skipped_position); TC_RUN(position_zero); TC_RUN(mixed_modes); return tc_end(); }
