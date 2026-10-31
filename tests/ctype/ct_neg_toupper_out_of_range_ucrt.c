/* TC
 * id:    ct_neg_toupper_out_of_range_ucrt
 * area:  ctype
 * kind:  negative
 * funcs: toupper tolower
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.4p1 (undefined outside EOF/unsigned char); MS docs toupper
 * note:  PROBES: the UCRT returns an out-of-range argument unchanged. 0x161 guards against low-byte truncation
 *        (would yield 'A' = 0x41).
 */
#include "tc.h"
#include <ctype.h>

static volatile int vc;

TC_CASE(toupper_256_is_256) { vc = 256; CHECK_EQ_I(toupper(vc), 256); }
TC_CASE(tolower_300_is_300) { vc = 300; CHECK_EQ_I(tolower(vc), 300); }
TC_CASE(toupper_minus2_is_minus2) { vc = -2; CHECK_EQ_I(toupper(vc), -2); }
TC_CASE(toupper_0x161_not_aliased_to_0x41) { vc = 0x161; CHECK_EQ_I(toupper(vc), 0x161); }
TC_CASE(tolower_0x141_not_aliased_to_0x61) { vc = 0x141; CHECK_EQ_I(tolower(vc), 0x141); }

int main(void)
{
    tc_begin();
    TC_RUN(toupper_256_is_256); TC_RUN(tolower_300_is_300); TC_RUN(toupper_minus2_is_minus2);
    TC_RUN(toupper_0x161_not_aliased_to_0x41); TC_RUN(tolower_0x141_not_aliased_to_0x61);
    return tc_end();
}
