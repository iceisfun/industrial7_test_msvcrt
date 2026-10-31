/* TC
 * id:    loc_neg_wctomb_s_unmappable
 * area:  locale
 * funcs: wctomb_s setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wctomb_s: EILSEQ, *pRetValue = -1 for unconvertible wide char
 * note:  IPH not asserted.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

static volatile wchar_t v_cyr = 0x0416;

TC_CASE(eilseq)
{
    char b[8]; int n = 77, r;
    if (!setlocale(LC_ALL, "English_United States.1252")) { setlocale(LC_ALL, "C"); TC_SKIP("no 1252"); }
    r = wctomb_s(&n, b, sizeof b, v_cyr);
    CHECK_EQ_I(r, EILSEQ);
    setlocale(LC_ALL, "C");
}
TC_CASE(retvalue_minus1)
{
    char b[8]; int n = 77;
    if (!setlocale(LC_ALL, "English_United States.1252")) { setlocale(LC_ALL, "C"); TC_SKIP("no 1252"); }
    (void)wctomb_s(&n, b, sizeof b, v_cyr);
    CHECK_EQ_I(n, -1);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(eilseq); TC_RUN(retvalue_minus1); return tc_end(); }
