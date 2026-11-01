/* TC
 * id:    pfi_neg_probe_invalid_len_combo
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification, Parameter Validation
 * note:  ucrt rejects repeated length modifiers ("%hhhd", "%lllld") by returning -1 and calling the invalid-parameter handler
 *        (observed, unchanged). A dangling '%' after a valid spec ("%ld%") is NOT rejected on Win10 22H2: returns 1, output "1", handler not called.
 *        Raw evidence (baseline): TC FAIL ...::dangling_percent n == -1 got=1 want=-1 ; tc_iph_count > 0
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
    CHECK_EQ_I(n, 1); CHECK_STR(b, "1"); CHECK_EQ_I(tc_iph_count, 0);   /* observed: trailing lone '%' is dropped silently */
}
int main(void) { tc_begin(); TC_RUN(hhh); TC_RUN(llll); TC_RUN(dangling_percent); return tc_end(); }
