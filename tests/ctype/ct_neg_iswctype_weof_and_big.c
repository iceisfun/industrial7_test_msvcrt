/* TC
 * id:    ct_neg_iswctype_weof_and_big
 * area:  ctype
 * kind:  negative
 * funcs: iswctype
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs iswctype; WEOF is 0xFFFF (U+FFFF noncharacter)
 * note:  WEOF is a valid wint_t argument and is not a member of any class. A zero mask selects no class.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static volatile unsigned int vc, vm;

TC_CASE(WEOF_all_masks_zero)
{
    static const unsigned short masks[] = { _UPPER, _LOWER, _DIGIT, _SPACE, _PUNCT, _CONTROL, _BLANK, _HEX, _ALPHA };
    size_t i, bad = 0;
    for (i = 0; i < sizeof masks / sizeof masks[0]; i++) { vc = 0xFFFF; vm = masks[i]; if (iswctype((wint_t)vc, (wctype_t)vm)) bad++; }
    CHECK_EQ_I(bad, 0);
}
TC_CASE(0xFFFF_any_mask_zero) { vc = 0xFFFF; vm = 0xFFFF; CHECK_EQ_I(iswctype((wint_t)vc, (wctype_t)vm), 0); }
TC_CASE(mask_zero_zero) { vc = 0; vm = 0; CHECK_EQ_I(iswctype((wint_t)vc, (wctype_t)vm), 0); vc = 'a'; CHECK_EQ_I(iswctype((wint_t)vc, (wctype_t)vm), 0); }

int main(void) { tc_begin(); TC_RUN(WEOF_all_masks_zero); TC_RUN(0xFFFF_any_mask_zero); TC_RUN(mask_zero_zero); return tc_end(); }
