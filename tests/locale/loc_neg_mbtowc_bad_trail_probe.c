/* TC
 * id:    loc_neg_mbtowc_bad_trail_probe
 * area:  locale
 * funcs: mbtowc mblen setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs mbtowc/mblen: -1 if not a valid multibyte character
 * note:  Win10 triage (class B, probe -> observed): ucrtbase mbtowc accepts lead 0x82 + 0x20 (returns 2, U+30FB, no errno) while mblen returns -1;
 *        a NUL trail gives -1/EILSEQ in mbtowc. Observed on Windows 10 22H2 build 19045; deviates from C11 7.22.7.1.
 *        Original PROBE: in cp932, lead 0x82 followed by 0x20 (invalid trail byte), count 2: mbtowc and mblen return -1 and mbtowc sets
 *        errno EILSEQ. Not certain: some CRT versions only validate the trail byte when it is NUL / count is short and may
 *        return 2 here. Skips if .932 is rejected.
 *        [scope] observed on ucrtbase 19045 only; the legacy CRTs listed in crt: are [unverified] until the P3 runs.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

static const char * volatile v_loc = ".932";
static int enter(void) { return setlocale(LC_ALL, (const char *)v_loc) != NULL; }
static char badtrail[3] = { (char)0x82, 0x20, 0 };

TC_CASE(mbtowc_invalid_trail)
{
    wchar_t wc = 0;
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    /* Win10 22H2 ucrtbase (observed): lead 0x82 + trail 0x20 is accepted: mbtowc returns 2, wc = U+30FB, errno untouched. */
    errno = 0;
    CHECK_EQ_I(mbtowc(&wc, badtrail, 2), 2);
    CHECK_EQ_U(wc, 0x30FB);
    CHECK_EQ_I(errno, 0);
    setlocale(LC_ALL, "C");
}
TC_CASE(mbtowc_nul_trail)
{
    wchar_t wc = 0; static char nultrail[3] = { (char)0x82, 0, 0 };
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    errno = 0;
    CHECK_EQ_I(mbtowc(&wc, nultrail, 2), -1);   /* observed: NUL trail is rejected with EILSEQ */
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(mblen_invalid_trail)
{
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mblen(badtrail, 2), -1);   /* observed: mblen rejects it although mbtowc/mbrlen accept it */
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(mbtowc_invalid_trail); TC_RUN(mbtowc_nul_trail); TC_RUN(mblen_invalid_trail); return tc_end(); }
