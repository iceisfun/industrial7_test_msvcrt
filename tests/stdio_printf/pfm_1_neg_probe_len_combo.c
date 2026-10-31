/* TC
 * id:    pfm_1_neg_probe_len_combo
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf size prefixes (no documented meaning for these length/conversion pairs)
 * note:  PROBES odd length/conversion combinations: %hhs, %Ld, %Lx, %hhp. Each is asserted only to be self-consistent: either
 *        it formats (return == strlen, no invalid-parameter call) or it is rejected (-1 with exactly one invalid-parameter call).
 *        (%n is covered by the pf?_neg_*percent_n* tests, not here.)
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static int ok_or_rejected(int r, const char *b, const char *want)
{
    if (r == -1) return tc_iph_count == 1;
    return tc_iph_count == 0 && (size_t)r == strlen(want) && strcmp(b, want) == 0;
}

static const char *volatile str = "abc";
static volatile int five = 5, ff = 255;
static void *volatile ptr = (void *)0;
TC_CASE(hhs) { char b[64]; int r; b[0] = 0; tc_iph_count = 0; r = sprintf(b, "%hhs", str); CHECK(ok_or_rejected(r, b, "abc")); }
TC_CASE(Ld) { char b[64]; int r; b[0] = 0; tc_iph_count = 0; r = sprintf(b, "%Ld", five); CHECK(ok_or_rejected(r, b, "5")); }
TC_CASE(Lx) { char b[64]; int r; b[0] = 0; tc_iph_count = 0; r = sprintf(b, "%Lx", ff); CHECK(ok_or_rejected(r, b, "ff")); }
TC_CASE(hhp) { char b[64]; int r; b[0] = 0; tc_iph_count = 0; r = sprintf(b, "%hhp", ptr); CHECK(ok_or_rejected(r, b, TC_X64 ? "0000000000000000" : "00000000")); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(hhs); TC_RUN(Ld); TC_RUN(Lx); TC_RUN(hhp);
    return tc_end();
}
