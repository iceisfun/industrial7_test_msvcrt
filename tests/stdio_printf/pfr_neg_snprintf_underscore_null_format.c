/* TC
 * id:    pfr_neg_snprintf_underscore_null_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf (NULL format: EINVAL, -1)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
TC_CASE(snprintf_null_fmt)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = _snprintf(b, sizeof b, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(snprintf_null_fmt);
    return tc_end();
}
