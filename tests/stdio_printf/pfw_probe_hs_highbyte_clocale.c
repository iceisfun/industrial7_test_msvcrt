/* TC
 * id:    pfw_probe_hs_highbyte_clocale
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf format (hs)
 * note:  Probing whether C-locale narrow-to-wide conversion of %hs maps the high byte 0xE9 1:1 to U+00E9 (ret==1, wc==0xE9). Only that is asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

TC_CASE(hs_E9_yields_U_00E9)
{
    wchar_t b[8] = {0}; static const char s[] = { (char)0xE9, 0 }; int n;
    n = _snwprintf(b, 8, L"%hs", s); b[7] = 0;
    CHECK_EQ_I(n, 1); CHECK_EQ_U(b[0], 0xE9);
}
int main(void) { tc_begin(); TC_RUN(hs_E9_yields_U_00E9); return tc_end(); }
