/* TC
 * id:    ct_iswctype_basic
 * area:  ctype
 * funcs: iswctype iswalpha iswdigit iswspace iswpunct iswcntrl iswxdigit iswupper iswlower
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs iswctype; ctype.h masks
 */
#include "tc.h"
#include <ctype.h>
#include <wchar.h>

static volatile wint_t wA = L'A', wa = L'a', w5 = L'5', wsp = L' ', wpu = L'!', wnl = L'\n', wf = L'f', wg = L'g';

TC_CASE(alpha_mask_on_letters) { CHECK(iswctype(wA, _ALPHA)); CHECK(iswctype(wa, _ALPHA)); CHECK(!iswctype(w5, _ALPHA)); CHECK(!iswctype(wsp, _ALPHA)); }
TC_CASE(digit_mask_on_digits) { CHECK(iswctype(w5, _DIGIT)); CHECK(!iswctype(wA, _DIGIT)); }
TC_CASE(upper_lower_masks) { CHECK(iswctype(wA, _UPPER)); CHECK(!iswctype(wa, _UPPER)); CHECK(iswctype(wa, _LOWER)); CHECK(!iswctype(wA, _LOWER)); }
TC_CASE(space_mask) { CHECK(iswctype(wsp, _SPACE)); CHECK(iswctype(wnl, _SPACE)); CHECK(!iswctype(wA, _SPACE)); }
TC_CASE(punct_mask) { CHECK(iswctype(wpu, _PUNCT)); CHECK(!iswctype(wA, _PUNCT)); CHECK(!iswctype(w5, _PUNCT)); }
TC_CASE(cntrl_mask) { CHECK(iswctype(wnl, _CONTROL)); CHECK(!iswctype(wA, _CONTROL)); }
TC_CASE(xdigit_mask) { CHECK(iswctype(wf, _HEX)); CHECK(iswctype(w5, _HEX)); CHECK(!iswctype(wg, _HEX)); }
TC_CASE(agrees_with_isw_for_0_127)
{
    volatile int i;
    for (i = 0; i < 128; i++) {
        wint_t c = (wint_t)i;
        CHECK_EQ_I((iswctype(c, _ALPHA) != 0), ((iswalpha)(c) != 0));
        CHECK_EQ_I((iswctype(c, _DIGIT) != 0), ((iswdigit)(c) != 0));
        CHECK_EQ_I((iswctype(c, _SPACE) != 0), ((iswspace)(c) != 0));
        CHECK_EQ_I((iswctype(c, _PUNCT) != 0), ((iswpunct)(c) != 0));
        CHECK_EQ_I((iswctype(c, _CONTROL) != 0), ((iswcntrl)(c) != 0));
        CHECK_EQ_I((iswctype(c, _HEX) != 0), ((iswxdigit)(c) != 0));
        CHECK_EQ_I((iswctype(c, _UPPER) != 0), ((iswupper)(c) != 0));
        CHECK_EQ_I((iswctype(c, _LOWER) != 0), ((iswlower)(c) != 0));
    }
}
int main(void) { tc_begin(); TC_RUN(alpha_mask_on_letters); TC_RUN(digit_mask_on_digits); TC_RUN(upper_lower_masks); TC_RUN(space_mask); TC_RUN(punct_mask); TC_RUN(cntrl_mask); TC_RUN(xdigit_mask); TC_RUN(agrees_with_isw_for_0_127); return tc_end(); }
