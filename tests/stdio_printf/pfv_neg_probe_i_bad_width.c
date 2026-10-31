/* TC
 * id:    pfv_neg_probe_i_bad_width
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf Size specification: I, I32, I64 are the documented I-prefixes
 * note:  PROBE: undocumented I-prefix widths (%I48d, %I16d) and a bare trailing "%I" are expected to fail with -1, EINVAL and
 *        one handler call (the 'I' is taken as pointer-size and the following digit is then not a conversion). The valid
 *        "%Id" control (documented) is the only positive assertion.
 *        crt narrowed to ucrtbase: pre-UCRT CRTs may print unparsable specs literally (see pfv_neg_probe_invalid_conv_char_pre140).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f48 = "%I48d";
static const char *volatile f16 = "%I16d";
static const char *volatile fI = "%I";
static const char *volatile fId = "%Id";
static volatile int v = 5;

TC_CASE(I48d) { char b[64]; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, f48, v); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(I16d) { char b[64]; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, f16, v); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(I_at_end) { char b[64]; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, fI); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(Id_control) { char b[64]; int r; tc_iph_count = 0; r = sprintf(b, fId, (INT_PTR)v); CHECK_EQ_I(r, 1); CHECK_STR(b, "5"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(I48d); TC_RUN(I16d); TC_RUN(I_at_end); TC_RUN(Id_control);
    return tc_end();
}
