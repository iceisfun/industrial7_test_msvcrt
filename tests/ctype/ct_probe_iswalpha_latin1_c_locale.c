/* TC
 * id:    ct_probe_iswalpha_latin1_c_locale
 * area:  ctype
 * funcs: iswalpha iswupper iswlower iswprint iswpunct
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  observed
 * ref:   MS docs iswalpha
 * note:  PROBE: isw* on Latin-1 code points in the C locale use the Unicode table (unlike narrow functions).
 *        Observed Win10 22H2 ucrtbase: iswprint(0xA0) == 0 (NBSP not printable in the ucrt wide table; deviates from Unicode/C11 7.30.2.1.8
 *        expectation); all other cases as originally expected. Stable.
 */
#include "tc.h"
#include <wctype.h>
#undef iswalpha
#undef iswupper
#undef iswlower
#undef iswprint
#undef iswpunct

static volatile wint_t w;

TC_CASE(iswalpha_0xE9_true) { w = 0xE9; CHECK(iswalpha(w) != 0); }
TC_CASE(iswupper_0xC9_true) { w = 0xC9; CHECK(iswupper(w) != 0); }
TC_CASE(iswlower_0xE9_true) { w = 0xE9; CHECK(iswlower(w) != 0); }
TC_CASE(iswalpha_0xD7_false) { w = 0xD7; CHECK_EQ_I(iswalpha(w), 0); }
TC_CASE(iswpunct_0xA1_true) { w = 0xA1; CHECK(iswpunct(w) != 0); }
TC_CASE(iswprint_0xA0_true)  /* name kept; observed value is false */ { w = 0xA0; CHECK_EQ_I(iswprint(w), 0); }

int main(void)
{
    tc_begin();
    TC_RUN(iswalpha_0xE9_true); TC_RUN(iswupper_0xC9_true); TC_RUN(iswlower_0xE9_true);
    TC_RUN(iswalpha_0xD7_false); TC_RUN(iswpunct_0xA1_true); TC_RUN(iswprint_0xA0_true);
    return tc_end();
}
