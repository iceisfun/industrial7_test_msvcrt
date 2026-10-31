/* TC
 * id:    loc_neg_wctomb_s_range_probe
 * area:  locale
 * funcs: wctomb_s setlocale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs wctomb_s
 * note:  PROBE: in cp932 converting U+3042 (2 bytes) into a 1-byte buffer returns ERANGE and invokes the invalid-parameter
 *        handler exactly once. The ERANGE code and the handler call are believed, not documented precisely.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

static volatile wchar_t v_hira = 0x3042;

TC_CASE(dbcs_into_1_byte_erange)
{
    char b[4]; int n = 77, r;
    if (!setlocale(LC_ALL, ".932")) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    r = wctomb_s(&n, b, 1, v_hira);
    CHECK_EQ_I(r, ERANGE);
    setlocale(LC_ALL, "C");
}
TC_CASE(iph_count)
{
    char b[4]; int n = 77, c0 = tc_iph_count;
    if (!setlocale(LC_ALL, ".932")) { setlocale(LC_ALL, "C"); TC_SKIP("no .932"); }
    (void)wctomb_s(&n, b, 1, v_hira);
    CHECK_EQ_I(tc_iph_count, c0 + 1);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(dbcs_into_1_byte_erange); TC_RUN(iph_count); return tc_end(); }
