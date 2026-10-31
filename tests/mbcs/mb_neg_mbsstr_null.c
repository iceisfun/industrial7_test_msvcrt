/* TC
 * id:    mb_neg_mbsstr_null
 * area:  mbcs
 * funcs: _mbsstr _setmbcp
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbsstr, Parameter validation
 * note:  PROBE: _mbsstr with a NULL argument invokes the invalid-parameter handler and returns NULL (not explicitly documented per function). errno not asserted. cp932 selected.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(null_haystack)
{
    static unsigned char n[] = "a";
    tc_iph_count = 0; CHECK(_mbsstr(NULL, n) == NULL); CHECK(tc_iph_count >= 1);
}
TC_CASE(null_needle)
{
    static unsigned char h[] = "a";
    tc_iph_count = 0; CHECK(_mbsstr(h, NULL) == NULL); CHECK(tc_iph_count >= 1);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no iph\n"); return 1; }
    TC_RUN(null_haystack); TC_RUN(null_needle); _setmbcp(old); return tc_end();
}
