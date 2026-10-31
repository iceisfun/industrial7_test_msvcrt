/* TC
 * id:    pfv_neg_invalid_conv_char
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs parameter validation; printf_s invalid format
 * note:  UCRT only: unknown conversion char is an invalid format -> handler, -1, EINVAL. Pre-UCRT CRTs: see pfv_neg_probe_invalid_conv_char_pre140.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static int ret, iph, err;
static void run(const char *f)
{
    char b[64]; tc_iph_count = 0; errno = 0;
    ret = sprintf(b, f); iph = tc_iph_count; err = errno;
}
TC_CASE(pct_y) { volatile const char *f = "%y"; run((const char *)f); CHECK_EQ_I(ret, -1); }
TC_CASE(pct_q_after_flags) { volatile const char *f = "ab%-5.2q"; run((const char *)f); CHECK_EQ_I(ret, -1); }
TC_CASE(iph_count) { volatile const char *f = "%y"; run((const char *)f); CHECK_EQ_I(iph, 1); }
TC_CASE(errno_einval) { volatile const char *f = "%y"; run((const char *)f); CHECK_EQ_I(err, EINVAL); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(pct_y); TC_RUN(pct_q_after_flags); TC_RUN(iph_count); TC_RUN(errno_einval); return tc_end(); }
