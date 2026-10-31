/* TC
 * id:    loc_mbtowc_wctomb_dbcs
 * area:  locale
 * funcs: mbtowc wctomb setlocale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  observed
 * ref:   MS docs mbtowc, wctomb; code page 932 (Shift-JIS)
 * note:  Lone lead byte case is in loc_neg_mbtowc_incomplete_dbcs. Skips if .932 is rejected.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>

static const char * volatile v_loc = ".932";
static int enter(void) { return setlocale(LC_ALL, (const char *)v_loc) != NULL; }
static unsigned char hira[3] = { 0x82, 0xA0, 0 };
static volatile wchar_t v_wa = 0x3042;

TC_CASE(mbtowc_dbcs)
{
    wchar_t wc = 0;
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mbtowc(&wc, (const char *)hira, 2), 2);
    CHECK_EQ_I(wc, 0x3042);
    setlocale(LC_ALL, "C");
}
TC_CASE(mbtowc_count_larger_than_needed)
{
    wchar_t wc = 0; char a[4] = { 'a', 0, 0, 0 };
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(mbtowc(&wc, a, 2), 1);
    CHECK_EQ_I(wc, L'a');
    wc = 0;
    CHECK_EQ_I(mbtowc(&wc, (const char *)hira, 3), 2);
    CHECK_EQ_I(wc, 0x3042);
    setlocale(LC_ALL, "C");
}
TC_CASE(wctomb_dbcs)
{
    unsigned char b[4] = { 0, 0, 0, 0 };
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(wctomb((char *)b, v_wa), 2);
    CHECK_EQ_U(b[0], 0x82);
    CHECK_EQ_U(b[1], 0xA0);
    setlocale(LC_ALL, "C");
}
TC_CASE(wctomb_ascii_one_byte)
{
    char b[4] = { 0, 0, 0, 0 };
    if (!enter()) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    CHECK_EQ_I(wctomb(b, L'a'), 1);
    CHECK_EQ_I(b[0], 'a');
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(mbtowc_dbcs); TC_RUN(mbtowc_count_larger_than_needed); TC_RUN(wctomb_dbcs); TC_RUN(wctomb_ascii_one_byte); return tc_end(); }
