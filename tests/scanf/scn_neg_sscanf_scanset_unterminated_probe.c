/* TC
 * id:    scn_neg_sscanf_scanset_unterminated_probe
 * area:  scanf
 * funcs: sscanf
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.21.6.2p12 (scanset ends at the closing ]); unterminated set is undefined
 * note:  PROBES '%[abc' (no closing bracket) and '%[]' (a leading ] is a member, so the set is unterminated).
 *        Safe cases: no crash, ret is 0 or EOF. Guess cases: invalid-parameter handler runs once.
 *        Destination is 16 bytes pre-filled with a sentinel and checked past the matched text.
 *        CORRECTED for Win10 22H2 ucrtbase (observed 2026-10-31): an unterminated scanset ('%[abc', '%[]') does not call the invalid parameter handler (count 0). Stable (2 runs).
 *        Observed on Win10 22H2 ucrtbase only; the same expectation on the other CRTs (>=80) is [unverified] and must be confirmed when those CRTs are run in the VM.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile in;
static const char *volatile fmt;

TC_CASE(unterminated_ret)  { char s[16]; int r; memset(s, 'Q', sizeof s); tc_iph_count = 0; in = "abc"; fmt = "%[abc"; r = sscanf(in, fmt, s); CHECK(r == 0 || r == EOF); CHECK(s[8] == 'Q'); }
TC_CASE(unterminated_iph)  { char s[16]; memset(s, 'Q', sizeof s); tc_iph_count = 0; in = "abc"; fmt = "%[abc"; sscanf(in, fmt, s); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(bracket_only_ret)  { char s[16]; int r; memset(s, 'Q', sizeof s); tc_iph_count = 0; in = "]]x"; fmt = "%[]"; r = sscanf(in, fmt, s); CHECK(r == 0 || r == EOF); CHECK(s[8] == 'Q'); }
TC_CASE(bracket_only_iph)  { char s[16]; memset(s, 'Q', sizeof s); tc_iph_count = 0; in = "]]x"; fmt = "%[]"; sscanf(in, fmt, s); CHECK_EQ_I(tc_iph_count, 0); }

int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP " TC_TEST_ID " no handler API\n"); return tc_end(); }
    TC_RUN(unterminated_ret); TC_RUN(unterminated_iph); TC_RUN(bracket_only_ret); TC_RUN(bracket_only_iph);
    return tc_end();
}
