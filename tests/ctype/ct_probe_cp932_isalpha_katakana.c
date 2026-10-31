/* TC
 * id:    ct_probe_cp932_isalpha_katakana
 * area:  ctype
 * funcs: isalpha ispunct setlocale isleadbyte
 * procs: 
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs isalpha; cp932
 * note:  PROBE: half-width katakana 0xB1 isalpha nonzero, 0xA1 ispunct nonzero in cp932; not lead byte. Skipped if locale missing.
 */
#include "tc.h"
#include <ctype.h>
#include <locale.h>

static volatile int c;

TC_CASE(setlocale_ok_or_skip) { if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("cp932 unavailable"); setlocale(LC_ALL, "C"); }
TC_CASE(isalpha_0xB1_probe_nonzero) { if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("cp932 unavailable"); c = 0xB1; CHECK(isalpha(c) != 0); setlocale(LC_ALL, "C"); }
TC_CASE(ispunct_0xA1_probe_nonzero) { if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("cp932 unavailable"); c = 0xA1; CHECK(ispunct(c) != 0); setlocale(LC_ALL, "C"); }
TC_CASE(isleadbyte_0xB1_zero) { if (!setlocale(LC_ALL, "Japanese_Japan.932")) TC_SKIP("cp932 unavailable"); c = 0xB1; CHECK_EQ_I(isleadbyte(c), 0); setlocale(LC_ALL, "C"); }
TC_CASE(restore_C_locale) { setlocale(LC_ALL, "C"); c = 0xB1; CHECK_EQ_I(isalpha(c), 0); }

int main(void)
{
    tc_begin();
    TC_RUN(setlocale_ok_or_skip); TC_RUN(isalpha_0xB1_probe_nonzero); TC_RUN(ispunct_0xA1_probe_nonzero);
    TC_RUN(isleadbyte_0xB1_zero); TC_RUN(restore_C_locale);
    return tc_end();
}
