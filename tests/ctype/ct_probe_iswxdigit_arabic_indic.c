/* TC
 * id:    ct_probe_iswxdigit_arabic_indic
 * area:  ctype
 * funcs: iswxdigit
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs iswxdigit
 * note:  PROBE: iswxdigit(U+0660 ARABIC-INDIC DIGIT ZERO) is zero (only ASCII hex digits). Genuinely uncertain: the
 *        NLS C1_XDIGIT flag may also be set for non-ASCII decimal digits.
 */
#include "tc.h"
#include <wctype.h>
#undef iswxdigit

static volatile wint_t w;

TC_CASE(iswxdigit_0x0660_zero) { w = 0x0660; CHECK_EQ_I(iswxdigit(w), 0); }

int main(void)
{
    tc_begin();
    TC_RUN(iswxdigit_0x0660_zero);
    return tc_end();
}
