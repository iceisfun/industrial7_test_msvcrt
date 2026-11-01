/* TC
 * id:    pfi_neg_probe_len_modifier_incomplete
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf Size field
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (non-secure sprintf, run twice): a format ending right after a size prefix ('%I', '%I64') or a prefix followed by a non-integer
 *        conversion ('%Iz') is NOT rejected: returns 0, writes an empty string, handler not called. The old expectation (-1 + handler) was wrong for the
 *        non-secure path (the secure variant rejects "%I64", see pfo_neg_probe_vsprintf_bad_length_modifiers::i64_end_s).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile fI = "%I", *volatile fI64 = "%I64", *volatile fIz = "%Iz";

TC_CASE(bare_I_end)  { char b[64]; int n; errno = 0; tc_iph_count = 0; b[0] = 'Q'; n = sprintf(b, fI, 1);   CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, ""); }
TC_CASE(I64_end)     { char b[64]; int n; errno = 0; tc_iph_count = 0; b[0] = 'Q'; n = sprintf(b, fI64, 1); CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, ""); }
TC_CASE(Iz)          { char b[64]; int n; errno = 0; tc_iph_count = 0; b[0] = 'Q'; n = sprintf(b, fIz, 1);  CHECK_EQ_I(n, 0); CHECK_EQ_I(tc_iph_count, 0); CHECK_STR(b, ""); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(bare_I_end); TC_RUN(I64_end); TC_RUN(Iz);
    return tc_end();
}
