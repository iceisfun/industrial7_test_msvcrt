/* TC
 * id:    loc_neg_utf8_wcrtomb_lone_surrogate
 * area:  locale
 * funcs: wcrtomb wcstombs setlocale
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.29.6.3.3 wcrtomb (EILSEQ if wc is not a valid wide character); UTF-16 well-formedness
 * note:  PROBE: in a UTF-8 locale, wchar_t is a UTF-16 unit. A lone HIGH surrogate is deliberately not tested (UCRT may
 *        buffer it in mbstate_t and return 0). A lone LOW surrogate L'\xDC00' with fresh state should give (size_t)-1/EILSEQ, and wcstombs on a string holding a lone low surrogate -1.
 */
#include "tc.h"
#include <locale.h>
#include <errno.h>
#include <stdlib.h>
#include <wchar.h>

static const char * volatile v_loc;
static volatile wchar_t v_wc;
static int utf8(void) { v_loc = ".UTF-8"; return setlocale(LC_ALL, v_loc) != NULL; }

TC_CASE(wcrtomb_lone_low)
{
    mbstate_t st; char b[16]; size_t i;
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    for (i = 0; i < sizeof st; i++) ((char *)&st)[i] = 0;
    v_wc = 0xDC00; errno = 0;
    CHECK_EQ_U(wcrtomb(b, v_wc, &st), (size_t)-1);
    CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(wcstombs_lone_low_string)
{
    wchar_t w[4]; char b[16];
    if (!utf8()) TC_SKIP("no UTF-8 locale");
    w[0] = L'a'; w[1] = 0; v_wc = 0xDC00; w[1] = v_wc; w[2] = L'b'; w[3] = 0;
    CHECK_EQ_U(wcstombs(b, w, 16), (size_t)-1);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); TC_RUN(wcrtomb_lone_low); TC_RUN(wcstombs_lone_low_string); setlocale(LC_ALL, "C"); return tc_end(); }
