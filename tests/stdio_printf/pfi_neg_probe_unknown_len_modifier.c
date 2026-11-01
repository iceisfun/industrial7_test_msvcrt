/* TC
 * id:    pfi_neg_probe_unknown_len_modifier
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf Size field (I32, I64, I only)
 * note:  '%I16d', '%I33d', '%I8d' are not valid size prefixes (docs: I32, I64, I only). Observed Win10 22H2 ucrtbase (plain sprintf, no validation): the '%I' is consumed, the rest is copied literally ("16d", "33d", "8d"), no EINVAL, no handler call.
 *        Raw evidence (baseline): TC FAIL ...::I16 n == -1 got=3 want=-1 ; errno == EINVAL got=0 ; tc_iph_count == 1 got=0 (I33 got=3, I8 got=2)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f16 = "%I16d", *volatile f33 = "%I33d", *volatile f8 = "%I8d";

TC_CASE(I16) { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, f16, 1); CHECK_EQ_I(n, 3); CHECK_STR(b, "16d"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(I33) { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, f33, 1); CHECK_EQ_I(n, 3); CHECK_STR(b, "33d"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(I8)  { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, f8, 1);  CHECK_EQ_I(n, 2); CHECK_STR(b, "8d"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(I16); TC_RUN(I33); TC_RUN(I8);
    return tc_end();
}
