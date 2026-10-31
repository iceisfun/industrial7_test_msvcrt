/* TC
 * id:    pfi_neg_probe_unknown_len_modifier
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf Size field (I32, I64, I only)
 * note:  PROBE: '%I16d', '%I33d', '%I8d' are not valid size prefixes. Expected on UCRT: format rejected (-1, EINVAL, invalid parameter handler). Unsure whether bare 'I' followed by a digit falls through to an invalid-conversion error.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f16 = "%I16d", *volatile f33 = "%I33d", *volatile f8 = "%I8d";

TC_CASE(I16) { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, f16, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(I33) { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, f33, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(I8)  { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, f8, 1);  CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(I16); TC_RUN(I33); TC_RUN(I8);
    return tc_end();
}
