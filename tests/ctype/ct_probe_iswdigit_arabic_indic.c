/* TC
 * id:    ct_probe_iswdigit_arabic_indic
 * area:  ctype
 * funcs: iswdigit iswalpha
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs iswdigit
 * note:  PROBE: Microsoft iswdigit accepts non-ASCII decimal digits (C1_DIGIT) and iswalpha rejects them. Fullwidth zero is the most uncertain. (iswxdigit split into ct_probe_iswxdigit_arabic_indic.)
 */
#include "tc.h"
#include <wctype.h>
#undef iswdigit
#undef iswalpha

static volatile wint_t w;

TC_CASE(iswdigit_0x0660_nonzero) { w = 0x0660; CHECK(iswdigit(w) != 0); }
TC_CASE(iswdigit_0xFF10_fullwidth_zero) { w = 0xFF10; CHECK(iswdigit(w) != 0); }
TC_CASE(iswalpha_0x0660_zero) { w = 0x0660; CHECK_EQ_I(iswalpha(w), 0); }

int main(void)
{
    tc_begin();
    TC_RUN(iswdigit_0x0660_nonzero); TC_RUN(iswdigit_0xFF10_fullwidth_zero); TC_RUN(iswalpha_0x0660_zero);
    return tc_end();
}
