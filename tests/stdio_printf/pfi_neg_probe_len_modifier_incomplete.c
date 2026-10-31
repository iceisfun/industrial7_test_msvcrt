/* TC
 * id:    pfi_neg_probe_len_modifier_incomplete
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf Size field
 * note:  PROBE: format ending right after a size prefix ('%I', '%I64') or a prefix followed by a non-integer conversion ('%Iz') is rejected with -1 and the invalid parameter handler. Expected: yes.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile fI = "%I", *volatile fI64 = "%I64", *volatile fIz = "%Iz";

TC_CASE(bare_I_end)  { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, fI, 1);   CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(I64_end)     { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, fI64, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(Iz)          { char b[64]; int n; errno = 0; tc_iph_count = 0; n = sprintf(b, fIz, 1);  CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(bare_I_end); TC_RUN(I64_end); TC_RUN(Iz);
    return tc_end();
}
