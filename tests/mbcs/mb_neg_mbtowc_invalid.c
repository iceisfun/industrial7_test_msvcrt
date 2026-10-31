/* TC
 * id:    mb_neg_mbtowc_invalid
 * area:  mbcs
 * funcs: mbtowc setlocale
 * kind:  negative
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.7.2; MS docs mbtowc (-1 on invalid sequence)
 * note:  Only return value asserted; errno in mb_probe_mblen_mbtowc_eilseq; output wchar is not checked.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>

TC_CASE(bad_trail)
{
    static const char s[] = { (char)0x82, 0x20, 0 };
    volatile const char *p = s; wchar_t wc = 0;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_I(mbtowc(&wc, (const char *)p, 2), -1);
    setlocale(LC_ALL, "C");
}
TC_CASE(lone_lead)
{
    static const char s[] = { (char)0x82, 0 };
    volatile const char *p = s; wchar_t wc = 0;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_I(mbtowc(&wc, (const char *)p, 1), -1);
    setlocale(LC_ALL, "C");
}

int main(void) { tc_begin(); TC_RUN(bad_trail); TC_RUN(lone_lead); setlocale(LC_ALL, "C"); return tc_end(); }
