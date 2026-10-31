/* TC
 * id:    pfr_neg_snprintf_c99_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: snprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs snprintf, _snprintf (NULL buffer with count>0 or NULL format: EINVAL, -1)
 * note:  snprintf(NULL,0,fmt) is valid (count only) and is a positive test elsewhere.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <string.h>

static int have_iph;
TC_CASE(null_buf_count5)
{
    char *volatile nb = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = snprintf(nb, 5, "x");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_fmt)
{
    char b[8]; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = snprintf(b, 5, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buf_null_fmt_count0)
{
    char *volatile nb = NULL; const char *volatile f = NULL; int r;
    if (!have_iph) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    r = snprintf(nb, 0, f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(null_buf_count5); TC_RUN(null_fmt); TC_RUN(null_buf_null_fmt_count0);
    return tc_end();
}
