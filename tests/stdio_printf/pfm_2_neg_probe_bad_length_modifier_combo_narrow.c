/* TC
 * id:    pfm_2_neg_probe_bad_length_modifier_combo_narrow
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf size prefixes (hh h l ll j z t L I I32 I64 w) - only these combinations are documented
 * note:  PROBES: undocumented prefixes '%llld' '%hhhd' '%Lld' '%hld' '%I3d' '%I33d' '%I8d' '%I65d' with the _s functions.
 *        The only assertion is self-consistency: either rejected (-1 and exactly one invalid-parameter call) or accepted
 *        (no handler call, result >= 0 and equal to the string length). Neither must crash or overrun the buffer
 *        (a guard byte after the 16-char buffer is verified). Each format in its own case; narrow variant only.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static volatile int five = 5;

static int ok_n(int r, const char *b, unsigned guard)
{
    size_t n = 0; while (n < 16 && b[n]) n++;
    if (guard != 0x5a) return 0;
    if (r == -1) return tc_iph_count == 1;
    return tc_iph_count == 0 && r >= 0 && (size_t)r == n;
}
static void run_n(const char *f)
{
    char b[17]; int r; b[16] = 0x5a; b[0] = 0; tc_iph_count = 0;
    r = sprintf_s(b, 16, f, five); CHECK(ok_n(r, b, (unsigned char)b[16]));
}
TC_CASE(n_lll) { run_n("%llld"); }
TC_CASE(n_hhh) { run_n("%hhhd"); }
TC_CASE(n_Ll) { run_n("%Lld"); }
TC_CASE(n_hl) { run_n("%hld"); }
TC_CASE(n_I3) { run_n("%I3d"); }
TC_CASE(n_I33) { run_n("%I33d"); }
TC_CASE(n_I8) { run_n("%I8d"); }
TC_CASE(n_I65) { run_n("%I65d"); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(n_lll); TC_RUN(n_hhh); TC_RUN(n_Ll); TC_RUN(n_hl); 
    TC_RUN(n_I3); TC_RUN(n_I33); TC_RUN(n_I8); TC_RUN(n_I65); 
    return tc_end();
}
