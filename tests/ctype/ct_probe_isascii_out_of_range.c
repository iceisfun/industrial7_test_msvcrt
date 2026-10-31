/* TC
 * id:    ct_probe_isascii_out_of_range
 * area:  ctype
 * funcs: __isascii
 * procs: isascii
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs __isascii, isascii
 * note:  PROBE: __isascii/isascii with ints outside 0..255 (256, 1000, -1). Docs only define the 0x00-0x7F range; expected 0 (definition is a range/mask test on the int value).
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
typedef int (__cdecl *isascii_t)(int);
static isascii_t p_isascii;

TC_CASE(c256_and_1000_zero)
{
    if (!p_isascii) TC_SKIP("isascii not exported");
    vc = 256; CHECK_EQ_I(p_isascii(vc), 0); CHECK_EQ_I((__isascii)(vc), 0);
    vc = 1000; CHECK_EQ_I(p_isascii(vc), 0); CHECK_EQ_I((__isascii)(vc), 0);
}
TC_CASE(minus1_zero) { vc = -1; CHECK_EQ_I((__isascii)(vc), 0); }

int main(void) { tc_begin(); p_isascii = (isascii_t)tc_crt_proc("isascii"); TC_RUN(c256_and_1000_zero); TC_RUN(minus1_zero); return tc_end(); }
