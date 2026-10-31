/* TC
 * id:    loc_neg_mbtowc_bad_trail_probe
 * area:  locale
 * funcs: mbtowc mblen setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs mbtowc/mblen: -1 if not a valid multibyte character
 * note:  PROBE: in cp932, lead 0x82 followed by 0x20 (invalid trail byte), count 2: mbtowc and mblen return -1 and mbtowc sets
 *        errno EILSEQ. Not certain: some CRT versions only validate the trail byte when it is NUL / count is short and may
 *        return 2 here. Skips if .932 is rejected.
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
    errno = 0;
    CHECK_EQ_I(mbtowc(&wc, badtrail, 2), -1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(mblen_invalid_trail)
{
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mblen(badtrail, 2), -1);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(mbtowc_invalid_trail); TC_RUN(mblen_invalid_trail); return tc_end(); }
