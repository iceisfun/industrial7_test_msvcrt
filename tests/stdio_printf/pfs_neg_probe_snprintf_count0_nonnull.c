/* TC
 * id:    pfs_neg_probe_snprintf_count0_nonnull
 * area:  stdio_printf
 * funcs: _snprintf
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf (behavior summary table says count==0 returns would-be length; legacy truncation says -1)
 * note:  PROBE: _snprintf(buf, 0, "%s", "abcdef") with a non-NULL buffer. Most likely -1 (nothing fits, legacy
 *        truncation rule), buffer untouched. The docs table suggests 6 instead; real Windows decides.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
static const char *volatile s6 = "abcdef";
TC_CASE(count0) {
    char b[4]; int n; memset(b, 0x7e, sizeof b);
    n = _snprintf(b, 0, "%s", s6);
    CHECK_EQ_I(n, -1); CHECK_EQ_U((unsigned char)b[0], 0x7e);
}
int main(void) { tc_begin(); TC_RUN(count0); return tc_end(); }
