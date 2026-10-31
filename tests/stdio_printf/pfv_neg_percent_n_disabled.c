/* TC
 * id:    pfv_neg_percent_n_disabled
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf: %n disabled by default, invalid parameter handler invoked
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static char b[64];
static volatile int n;
static int ret, iph, err;

static void run(void)
{
    n = 0x5a5a; tc_iph_count = 0; errno = 0;
    ret = sprintf(b, "ab%n", (int *)&n);
    iph = tc_iph_count; err = errno;
}
TC_CASE(iph_called) { run(); CHECK_EQ_I(iph, 1); }
TC_CASE(returns_minus1) { run(); CHECK_EQ_I(ret, -1); }
TC_CASE(target_untouched) { run(); CHECK_EQ_I(n, 0x5a5a); }
TC_CASE(errno_einval) { run(); CHECK_EQ_I(err, EINVAL); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(iph_called); TC_RUN(returns_minus1); TC_RUN(target_untouched); TC_RUN(errno_einval); return tc_end(); }
