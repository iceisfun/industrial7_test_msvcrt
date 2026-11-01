/* TC
 * id:    mb_neg_mbtowc_invalid
 * area:  mbcs
 * funcs: mbtowc setlocale
 * kind:  negative
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.22.7.2; MS docs mbtowc (-1 on invalid sequence)
 * note:  Only return value asserted; errno in mb_probe_mblen_mbtowc_eilseq; output wchar is not checked.
 *        Observed Win10 22H2 ucrtbase (CP932): mbtowc(0x82 0x20, 2) returns 2 (non-mapping trail accepted, deviates from C11 7.22.7.2),
 *        mbtowc(0x82 0x00, 2) returns -1 (EILSEQ). bad_trail now uses 0x82 0x00; case trail_20_accepted pins the deviation.
 *        Stable (static table).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <stdio.h>

TC_CASE(bad_trail)
{
    static const char s[] = { (char)0x82, 0x00, 0 };   /* lead + NUL (0x82 0x20 is accepted on Windows) */
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

TC_CASE(trail_20_accepted)
{
    static const char s[] = { (char)0x82, 0x20, 0 };
    volatile const char *p = s; wchar_t wc = 0;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    CHECK_EQ_I(mbtowc(&wc, (const char *)p, 2), 2);   /* observed; C11 would give -1 */
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(bad_trail); TC_RUN(lone_lead); TC_RUN(trail_20_accepted); setlocale(LC_ALL, "C"); return tc_end(); }
