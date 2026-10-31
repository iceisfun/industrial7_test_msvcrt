/* TC
 * id:    mb_neg_probe_mblen_mbtowc_eilseq
 * area:  mbcs
 * funcs: mblen mbtowc setlocale
 * kind:  negative
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs mblen, mbtowc
 * note:  PROBE: errno == EILSEQ after mblen/mbtowc on an invalid DBCS sequence (0x82 0x20) in locale .932.
 *        Believed set by ucrt and VS2005+; msvcrt.dll (VC6 lineage) may leave errno untouched.
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
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(mbtowc_eilseq)
{
    static const char s[] = { (char)0x82, 0x20, 0 };
    volatile const char *p = s; wchar_t wc = 0;
    if (!setlocale(LC_CTYPE, ".932")) TC_SKIP("cp932 locale unavailable");
    errno = 0;
    CHECK_EQ_I(mbtowc(&wc, (const char *)p, 2), -1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}

int main(void) { tc_begin(); TC_RUN(mblen_eilseq); TC_RUN(mbtowc_eilseq); setlocale(LC_ALL, "C"); return tc_end(); }
