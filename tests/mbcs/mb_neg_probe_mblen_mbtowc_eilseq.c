/* TC
 * id:    mb_neg_probe_mblen_mbtowc_eilseq
 * area:  mbcs
 * funcs: mblen mbtowc setlocale
 * kind:  negative
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs mblen, mbtowc
 * note:  PROBE: errno == EILSEQ after mblen/mbtowc on an invalid DBCS sequence (0x82 0x20) in locale .932.
 *        Believed set by ucrt and VS2005+; msvcrt.dll (VC6 lineage) may leave errno untouched.
 *        Observed Win10 22H2 ucrtbase (CP932): mblen returns -1 for 0x82 0x20 but does NOT set errno (stays 0); mbtowc(0x82 0x20, 2)
 *        returns 2 (accepted), while mbtowc(0x82 0x00, 2) returns -1 with errno == EILSEQ. mbtowc_eilseq therefore uses 0x82 0x00. Stable.
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

TC_CASE(mblen_eilseq)
{
    static const char s[] = { (char)0x82, 0x20, 0 };
    volatile const char *p = s;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    errno = 0;
    CHECK_EQ_I(mblen((const char *)p, 2), -1);
    CHECK_EQ_I(errno, 0);   /* observed: mblen does not set EILSEQ */
    setlocale(LC_ALL, "C");
}
TC_CASE(mbtowc_eilseq)
{
    static const char s[] = { (char)0x82, 0x00, 0 };   /* 0x82 0x20 is accepted on Windows */
    volatile const char *p = s; wchar_t wc = 0;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    errno = 0;
    CHECK_EQ_I(mbtowc(&wc, (const char *)p, 2), -1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}

int main(void) { tc_begin(); TC_RUN(mblen_eilseq); TC_RUN(mbtowc_eilseq); setlocale(LC_ALL, "C"); return tc_end(); }
