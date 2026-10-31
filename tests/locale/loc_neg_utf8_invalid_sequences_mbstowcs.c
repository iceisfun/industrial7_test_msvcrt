/* TC
 * id:    loc_neg_utf8_invalid_sequences_mbstowcs
 * area:  locale
 * funcs: mbstowcs mbrlen mblen setlocale
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.7.1 mblen, 7.22.8.1 mbstowcs, 7.29.6.3.1 mbrlen (-2 incomplete, -1 EILSEQ); MS docs mbstowcs, mbrlen
 * note:  Needs the UTF-8 locale (Windows 10 1803+); skipped otherwise. mblen has no state so an incomplete sequence is -1.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <stdlib.h>
#include <wchar.h>

static const char * volatile v_loc;
static const char * volatile v_ff = "\xFF";
static const char * volatile v_trunc = "\xE2\x82";
static int utf8(void) { v_loc = ".UTF-8"; return setlocale(LC_ALL, v_loc) != NULL; }
static void zero(mbstate_t *s) { size_t i; for (i = 0; i < sizeof *s; i++) ((char *)s)[i] = 0; }

TC_CASE(mbstowcs_ff)
{
    wchar_t w[8];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    errno = 0; CHECK_EQ_U(mbstowcs(w, v_ff, 8), (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    errno = 0; CHECK_EQ_U(mbstowcs(NULL, v_ff, 0), (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(mbstowcs_truncated)
{
    wchar_t w[8];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    errno = 0; CHECK_EQ_U(mbstowcs(w, v_trunc, 8), (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(mblen_truncated_minus1)
{
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    CHECK_EQ_I(mblen(v_trunc, 2), -1);
    CHECK_EQ_I(mblen(v_ff, 1), -1);
    setlocale(LC_ALL, "C");
}
TC_CASE(mbrlen_truncated_minus2)
{
    mbstate_t st;
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    zero(&st);
    CHECK_EQ_U(mbrlen(v_trunc, 2, &st), (size_t)-2);
    zero(&st);
    errno = 0; CHECK_EQ_U(mbrlen(v_ff, 1, &st), (size_t)-1); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(mbstowcs_ff); TC_RUN(mbstowcs_truncated); TC_RUN(mblen_truncated_minus1); TC_RUN(mbrlen_truncated_minus2);
    setlocale(LC_ALL, "C");
    return tc_end();
}
