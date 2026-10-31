/* TC
 * id:    ct_neg_probe_toupper_msvcr_out_of_range_low_byte
 * area:  ctype
 * kind:  negative
 * funcs: toupper tolower
 * crt:   msvcr120
 * tier:  p1
 * conf:  probe
 * ref:   MS docs toupper/tolower (argument must be EOF or 0..255)
 * note:  PROBES that msvcr120 toupper/tolower given out-of-range values do not alias onto the low byte
 *        (0x161 must not become 0x41, 0x141 must not become 0x61). Documented behaviour is undefined; only
 *        survival and non-aliasing are asserted.
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;
TC_CASE(toupper_not_aliased) { vc = 0x161; CHECK(toupper(vc) != 0x41); }
TC_CASE(tolower_not_aliased) { vc = 0x141; CHECK(tolower(vc) != 0x61); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(toupper_not_aliased); TC_RUN(tolower_not_aliased);
    return tc_end();
}
