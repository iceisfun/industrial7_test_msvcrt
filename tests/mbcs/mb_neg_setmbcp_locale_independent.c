/* TC
 * id:    mb_neg_setmbcp_locale_independent
 * area:  mbcs
 * kind:  negative
 * funcs: _setmbcp _getmbcp mbtowc setlocale
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _setmbcp, mbtowc, setlocale
 * note:  PROBE: _setmbcp(932) affects only the _mbs* functions; mbtowc follows the C locale. In the "C" locale
 *        {0x82,0xA0} must NOT decode as one 2-byte character (single-byte locale, result 1), and after
 *        setlocale(LC_ALL,".932") it decodes to U+3042. Skips if codepage 932 is unavailable.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>
#include <mbctype.h>

TC_CASE(c_locale_not_two_byte)
{
    int old = _getmbcp(); wchar_t wc = 0; int r;
    static const char s[3] = { (char)0x82, (char)0xA0, 0 };
    CHECK(setlocale(LC_ALL, "C") != NULL);
    if (_setmbcp(932) != 0) { _setmbcp(old); TC_SKIP("cp932 unavailable"); }
    CHECK_EQ_I(_getmbcp(), 932);
    r = mbtowc(&wc, s, 2);
    CHECK(r != 2);
    CHECK_EQ_I(MB_CUR_MAX, 1);
    _setmbcp(old);
}
TC_CASE(locale_932_decodes)
{
    int old = _getmbcp(); wchar_t wc = 0; int r;
    static const char s[3] = { (char)0x82, (char)0xA0, 0 };
    if (!setlocale(LC_ALL, ".932")) { setlocale(LC_ALL, "C"); TC_SKIP("locale .932 unavailable"); }
    r = mbtowc(&wc, s, 2);
    CHECK_EQ_I(r, 2);
    CHECK_EQ_U(wc, 0x3042);
    setlocale(LC_ALL, "C");
    _setmbcp(old);
}
int main(void)
{
    tc_begin();
    TC_RUN(c_locale_not_two_byte); TC_RUN(locale_932_decodes);
    return tc_end();
}
