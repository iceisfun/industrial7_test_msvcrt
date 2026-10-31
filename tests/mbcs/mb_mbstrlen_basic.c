/* TC
 * id:    mb_mbstrlen_basic
 * area:  mbcs
 * funcs: _mbstrlen setlocale _setmbcp
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _mbstrlen: uses the current locale (LC_CTYPE), not the _setmbcp page
 * note:  Selects the Japanese cp932 locale; skips if not available.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>

static volatile int v_cp = 932;

TC_CASE(strings) {
    static const unsigned char e[] = "", h[] = "hello", m[] = { 0x82, 0xA0, 'a', 0x83, 0x41, 0 }, k[] = { 0xB1, 0xB2, 0 };
    if (!setlocale(LC_CTYPE, "Japanese_Japan.932")) TC_SKIP("cp932 locale unavailable");
    _setmbcp(v_cp);
    CHECK_EQ_U(_mbstrlen((const char *)e), 0); CHECK_EQ_U(_mbstrlen((const char *)h), 5);
    CHECK_EQ_U(_mbstrlen((const char *)m), 3); CHECK_EQ_U(_mbstrlen((const char *)k), 2);
    setlocale(LC_ALL, "C");
}

int main(void) { tc_begin(); TC_RUN(strings); setlocale(LC_ALL, "C"); _setmbcp(_MB_CP_ANSI); return tc_end(); }
