/* TC
 * id:    pfi_neg_probe_invalid_len_combo
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification, Parameter Validation
 * note:  PROBE: ucrt rejects repeated length modifiers ("%hhhd", "%lllld") and a dangling '%' after a valid
 *        spec ("%ld%") by returning -1 and calling the invalid-parameter handler.
 */
#include "tc.h"
#include <stdio.h>

static const char * volatile f_hhh = "%hhhd";
static const char * volatile f_llll = "%lllld";
static const char * volatile f_dangling = "%ld%";

TC_CASE(hhh)
{
    char b[64]; int n; tc_iph_install(); tc_iph_count = 0;
    n = sprintf(b, f_hhh, 1);
    CHECK_EQ_I(n, -1); CHECK(tc_iph_count > 0);
}
TC_CASE(llll)
{
    char b[64]; int n; tc_iph_install(); tc_iph_count = 0;
    n = sprintf(b, f_llll, 1LL);
    CHECK_EQ_I(n, -1); CHECK(tc_iph_count > 0);
}
TC_CASE(dangling_percent)
{
    char b[64]; int n; tc_iph_install(); tc_iph_count = 0;
    n = sprintf(b, f_dangling, 1L);
    CHECK_EQ_I(n, -1); CHECK(tc_iph_count > 0);
}
int main(void) { tc_begin(); TC_RUN(hhh); TC_RUN(llll); TC_RUN(dangling_percent); return tc_end(); }
