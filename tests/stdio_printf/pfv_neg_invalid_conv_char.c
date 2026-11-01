/* TC
 * id:    pfv_neg_invalid_conv_char
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs parameter validation; printf_s invalid format
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "pct_y ret == -1 got=1", "pct_q_after_flags ret == -1 got=3", "iph_count iph == 1 got=0", "errno_einval
 *        err == EINVAL got=0"; zzp_probe2 "sprintf %y"): ucrt does not treat an unknown conversion character as an invalid parameter: "%y" prints "y"
 *        (ret 1) and "ab%-5.2q" prints "abq" (ret 3; % flags width precision dropped), no handler call, errno unchanged. The MS-docs-based belief
 *        (-1/EINVAL/handler) is contradicted by the reference. C11 7.21.6.1p9: invalid conversion = UB, so conforming. Stable.
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
TC_CASE(pct_y) { volatile const char *f = "%y"; run((const char *)f); CHECK_EQ_I(ret, 1); }
TC_CASE(pct_q_after_flags) { volatile const char *f = "ab%-5.2q"; run((const char *)f); CHECK_EQ_I(ret, 3); }
TC_CASE(iph_not_called) { volatile const char *f = "%y"; run((const char *)f); CHECK_EQ_I(iph, 0); }
TC_CASE(errno_unchanged) { volatile const char *f = "%y"; run((const char *)f); CHECK_EQ_I(err, 0); }

int main(void) { tc_begin(); tc_iph_install(); TC_RUN(pct_y); TC_RUN(pct_q_after_flags); TC_RUN(iph_not_called); TC_RUN(errno_unchanged); return tc_end(); }
