/* TC
 * id:    pfm_3_neg_conv_n_ret_errno_probe
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _get_printf_count_output
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs Parameter validation (invalid parameter: errno = EINVAL, function returns an error value); printf type field n
 * note:  PROBE: after the handler returns for a %n variant carrying flags/width/precision, sprintf returns -1 and errno is EINVAL.
 *        Skips when %n is enabled.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

TC_DECL_PROC(int, _get_printf_count_output, (void));
static int enabled;
static const char * volatile vf;

TC_CASE(plain)      { char b[48]; int n = 1, r; if (enabled) TC_SKIP("%n enabled"); tc_iph_count = 0; errno = 0; vf = "%n"; r = sprintf(b, vf, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(minus)      { char b[48]; int n = 1, r; if (enabled) TC_SKIP("%n enabled"); tc_iph_count = 0; errno = 0; vf = "%-n"; r = sprintf(b, vf, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(width)      { char b[48]; int n = 1, r; if (enabled) TC_SKIP("%n enabled"); tc_iph_count = 0; errno = 0; vf = "%5n"; r = sprintf(b, vf, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(star_width) { char b[48]; int n = 1, r; if (enabled) TC_SKIP("%n enabled"); tc_iph_count = 0; errno = 0; vf = "%*n"; r = sprintf(b, vf, 3, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(prec)       { char b[48]; int n = 1, r; if (enabled) TC_SKIP("%n enabled"); tc_iph_count = 0; errno = 0; vf = "%.3n"; r = sprintf(b, vf, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(star_prec)  { char b[48]; int n = 1, r; if (enabled) TC_SKIP("%n enabled"); tc_iph_count = 0; errno = 0; vf = "%.*n"; r = sprintf(b, vf, 3, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_LOAD_PROC(_get_printf_count_output);
    enabled = tc_p__get_printf_count_output && tc_p__get_printf_count_output() != 0;
    TC_RUN(plain); TC_RUN(minus); TC_RUN(width); TC_RUN(star_width); TC_RUN(prec); TC_RUN(star_prec);
    return tc_end();
}
