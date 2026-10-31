/* TC
 * id:    pff_neg_gcvt_null_buffer
 * area:  stdio_printf
 * funcs: _gcvt
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _gcvt: NULL buffer is an invalid parameter
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(null_buf)
{
    char *r;
    if (!tc_iph_install()) TC_SKIP("no invalid parameter handler API");
    tc_iph_count = 0;
    r = _gcvt(1.5, 6, NULL);
    CHECK(r == NULL); CHECK_EQ_I(tc_iph_count, 1);
}

int main(void) { tc_begin(); TC_RUN(null_buf); return tc_end(); }
