/* TC
 * id:    mb_neg_mbschr_null
 * area:  mbcs
 * funcs: _mbschr _setmbcp
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _mbschr (parameter validation), Parameter validation
 * note:  PROBE: _mbschr(NULL,c) invokes the invalid-parameter handler, returns NULL and sets errno=EINVAL (generic CRT parameter validation; not explicitly documented per function). cp932 selected so the DBCS path is taken.
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(null_string)
{
    volatile unsigned int c = 'a'; unsigned char *r;
    tc_iph_count = 0; errno = 0;
    r = _mbschr(NULL, c);
    CHECK(r == NULL); CHECK(tc_iph_count >= 1); CHECK_EQ_I(errno, EINVAL);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__setup no iph\n"); return 1; }
    TC_RUN(null_string); _setmbcp(old); return tc_end();
}
