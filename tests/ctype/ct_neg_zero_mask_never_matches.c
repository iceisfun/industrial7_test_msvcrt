/* TC
 * id:    ct_neg_zero_mask_never_matches
 * area:  ctype
 * funcs: _isctype iswctype
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _isctype/iswctype (nonzero only if a mask bit matches)
 * note:  An empty mask can never match.
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

TC_CASE(isctype_zero_mask) { volatile int i, bad = 0; for (i = -1; i <= 255; i++) if (_isctype(i, 0)) bad++; CHECK_EQ_I(bad, 0); }
TC_CASE(iswctype_zero_mask) { volatile int i, bad = 0; for (i = 0; i <= 255; i++) if (iswctype((wint_t)i, 0)) bad++; CHECK_EQ_I(bad, 0); }
TC_CASE(letter_not_in_digit_space_punct_cntrl_masks)
{
    volatile int a = 'a';
    CHECK(!_isctype(a, _DIGIT | _SPACE | _PUNCT | _CONTROL | _UPPER));
}
int main(void) { tc_begin(); TC_RUN(isctype_zero_mask); TC_RUN(iswctype_zero_mask); TC_RUN(letter_not_in_digit_space_punct_cntrl_masks); return tc_end(); }
