/* TC
 * id:    loc_neg_wctomb_s_small_buffer
 * area:  locale
 * funcs: wctomb_s setlocale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs wctomb_s: success writing 1 byte into a 1-byte buffer; ERANGE cases are in loc_neg_wctomb_s_range_probe
 * note:  boundary: exactly-sized buffer must succeed.
 */
#include "tc.h"
#include <locale.h>
#include <stdlib.h>
#include <errno.h>

static volatile wchar_t v_euro = 0x20AC;

TC_CASE(sbcs_into_1_byte_ok)
{
    unsigned char b[4] = {0}; int n = 77, r;
    if (!setlocale(LC_ALL, "English_United States.1252")) { setlocale(LC_ALL, "C"); TC_SKIP("no 1252"); }
    r = wctomb_s(&n, (char *)b, 1, v_euro);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(n, 1);
    CHECK_EQ_U(b[0], 0x80);
    setlocale(LC_ALL, "C");
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(sbcs_into_1_byte_ok); return tc_end(); }
