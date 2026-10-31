/* TC
 * id:    loc_probe_wcstombs_c_latin1
 * area:  locale
 * funcs: wcstombs setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   none; reasoning from the C-locale narrowing loop (wchar <= 0xFF stored as a byte)
 * note:  PROBE: in the "C" locale, does wcstombs narrow U+00E9 / U+00FF to bytes 0xE9 / 0xFF (Latin-1) and fail for U+20AC?
 *        Pairs with the mbstowcs high-byte probe (symmetric Latin-1 widening in the C locale).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static wchar_t w_e[] = L"\x00e9", w_ff[] = L"\x00ff", w_eur[] = L"\x20ac";

TC_CASE(e_acute_narrows)
{
    unsigned char b[4] = {0}; size_t n;
    setlocale(LC_ALL, "C");
    n = wcstombs((char *)b, w_e, 4);
    CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0xE9);
}
TC_CASE(ff_narrows)
{
    unsigned char b[4] = {0}; size_t n = wcstombs((char *)b, w_ff, 4);
    CHECK_EQ_U(n, 1); CHECK_EQ_U(b[0], 0xFF);
}
TC_CASE(euro_fails)
{
    char b[4] = {0}; size_t n = wcstombs(b, w_eur, 4);
    CHECK_EQ_U(n, (size_t)-1);
}
int main(void)
{
    tc_begin();
    TC_RUN(e_acute_narrows); TC_RUN(ff_narrows); TC_RUN(euro_fails);
    return tc_end();
}
