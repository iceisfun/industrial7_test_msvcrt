/* TC
 * id:    gmc_alnum_l_alpha_fullwidth_probe
 * area:  mbcs
 * funcs: _ismbcalpha_l _ismbcalnum_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcalpha
 * note:  PROBE: fullwidth Latin capital A (0x8260) is alpha and alnum via the _l variants under cp932 (NULL locale = current).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
TC_CASE(fw_alpha) { CHECK_EQ_I(_setmbcp(932), 0); v = 0x8260; CHECK(_ismbcalpha_l(v, NULL) != 0); CHECK(_ismbcalnum_l(v, NULL) != 0); }
int main(void) { tc_begin(); TC_RUN(fw_alpha); _setmbcp(-3); return tc_end(); }
