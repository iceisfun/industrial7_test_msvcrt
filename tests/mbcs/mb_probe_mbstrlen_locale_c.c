/* TC
 * id:    mb_probe_mbstrlen_locale_c
 * area:  mbcs
 * funcs: _mbstrlen _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   probe
 * note:  PROBE: _mbstrlen of a DBCS pair under _setmbcp(932) while the locale is the C locale. ucrt uses locale MB_CUR_MAX (1) so likely returns 2; older CRTs use the mb codepage and return 1. Asserts only 1 or 2.
 */
#include "tc.h"
#include <locale.h>
#include <stdio.h>
#include <mbstring.h>
#include <mbctype.h>
#include <stdlib.h>

TC_CASE(c_locale_dbcs)
{
    static const unsigned char s[] = { 0x82, 0xA0, 0 };
    const char *volatile p = (const char *)s;
    size_t r;
    setlocale(LC_ALL, "C");
    if (_setmbcp(932) != 0) TC_SKIP("cp 932 unavailable");
    r = _mbstrlen(p);
    CHECK(r == 1 || r == 2);
    _setmbcp(_MB_CP_ANSI);
}
int main(void) { tc_begin(); TC_RUN(c_locale_dbcs); return tc_end(); }
