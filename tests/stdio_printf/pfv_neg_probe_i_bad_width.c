/* TC
 * id:    pfv_neg_probe_i_bad_width
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf Size specification: I, I32, I64 are the documented I-prefixes
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "I48d r == -1 got=3", "I16d ... got=3", "I_at_end r == -1 got=0"; zzp_probe2 "%I48d: r=3 b=[48d]", "%I16d: r=3
 *        b=[16d]", "%I32d: r=1 b=[5]", "%I64d: r=1 b=[5]", "sprintf %I: r=0 b=[]"): after the I prefix only 32 and 64 are width specs; for "%I48d"
 *        the I is consumed as a pointer-size prefix and "48d" is then printed literally (no valid conversion) -> 3 chars "48d", same for "%I16d";
 *        a bare trailing "%I" prints nothing (r=0). None of these is an invalid-parameter condition (no handler, errno unchanged). Old belief
 *        wrong; the %Id control (documented) is unchanged. C: Microsoft extension, no ISO text. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile f48 = "%I48d";
static const char *volatile f16 = "%I16d";
static const char *volatile fI = "%I";
static const char *volatile fId = "%Id";
static volatile int v = 5;

TC_CASE(I48d) { char b[64]; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, f48, v); CHECK_EQ_I(r, 3); CHECK_STR(b, "48d"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(I16d) { char b[64]; int r; errno = 0; tc_iph_count = 0; r = sprintf(b, f16, v); CHECK_EQ_I(r, 3); CHECK_STR(b, "16d"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(I_at_end) { char b[64]; int r; errno = 0; tc_iph_count = 0; b[0] = 1; r = sprintf(b, fI); CHECK_EQ_I(r, 0); CHECK_EQ_I(b[0], 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(Id_control) { char b[64]; int r; tc_iph_count = 0; r = sprintf(b, fId, (INT_PTR)v); CHECK_EQ_I(r, 1); CHECK_STR(b, "5"); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(I48d); TC_RUN(I16d); TC_RUN(I_at_end); TC_RUN(Id_control);
    return tc_end();
}
