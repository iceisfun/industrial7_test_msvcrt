/* TC
 * id:    ct_probe_isw_greek_cjk_c_locale
 * area:  ctype
 * funcs: iswalpha iswupper iswlower iswspace iswcntrl
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs iswalpha
 * note:  PROBE: classification above U+00FF in the C locale via the Unicode table; iswcntrl(U+2028) is the most uncertain.
 */
#include "tc.h"
#include <wctype.h>
#undef iswalpha
#undef iswupper
#undef iswlower
#undef iswspace
#undef iswcntrl

static volatile wint_t w;

TC_CASE(iswalpha_0x0391_greek_alpha) { w = 0x0391; CHECK(iswalpha(w) != 0); }
TC_CASE(iswupper_0x0391) { w = 0x0391; CHECK(iswupper(w) != 0); }
TC_CASE(iswlower_0x03B1) { w = 0x03B1; CHECK(iswlower(w) != 0); }
TC_CASE(iswalpha_0x4E00_cjk) { w = 0x4E00; CHECK(iswalpha(w) != 0); }
TC_CASE(iswspace_0x2003_em_space) { w = 0x2003; CHECK(iswspace(w) != 0); }
TC_CASE(iswspace_0x3000_ideographic_space) { w = 0x3000; CHECK(iswspace(w) != 0); }
TC_CASE(iswcntrl_0x2028_probe) { w = 0x2028; CHECK(iswcntrl(w) != 0); }

int main(void)
{
    tc_begin();
    TC_RUN(iswalpha_0x0391_greek_alpha); TC_RUN(iswupper_0x0391); TC_RUN(iswlower_0x03B1);
    TC_RUN(iswalpha_0x4E00_cjk); TC_RUN(iswspace_0x2003_em_space);
    TC_RUN(iswspace_0x3000_ideographic_space); TC_RUN(iswcntrl_0x2028_probe);
    return tc_end();
}
