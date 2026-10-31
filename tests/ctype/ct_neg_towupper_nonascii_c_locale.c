/* TC
 * id:    ct_neg_towupper_nonascii_c_locale
 * area:  ctype
 * funcs: towupper towlower toupper
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.4.2.2 (C locale: only the 26 ASCII letters have case)
 * note:  PROBE: in the "C" locale the CRT does not case-map non-ASCII wide chars (and toupper(0xE9) is unchanged).
 *        Each character is its own case so one wrong guess is isolated.
 */
#include "tc.h"
#include <ctype.h>
#include <wctype.h>

static volatile int vc;

TC_CASE(towupper_0xE9_unchanged) { vc = 0xE9; CHECK_EQ_U(towupper((wint_t)vc), 0xE9); }
TC_CASE(towlower_0xC9_unchanged) { vc = 0xC9; CHECK_EQ_U(towlower((wint_t)vc), 0xC9); }
TC_CASE(towupper_0x03B1_greek_unchanged) { vc = 0x3B1; CHECK_EQ_U(towupper((wint_t)vc), 0x3B1); }
TC_CASE(towupper_0x0430_cyrillic_unchanged) { vc = 0x430; CHECK_EQ_U(towupper((wint_t)vc), 0x430); }
TC_CASE(toupper_0xE9_unchanged) { vc = 0xE9; CHECK_EQ_I(toupper(vc), 0xE9); }

int main(void)
{
    tc_begin();
    TC_RUN(towupper_0xE9_unchanged); TC_RUN(towlower_0xC9_unchanged);
    TC_RUN(towupper_0x03B1_greek_unchanged); TC_RUN(towupper_0x0430_cyrillic_unchanged);
    TC_RUN(toupper_0xE9_unchanged);
    return tc_end();
}
