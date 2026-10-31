/* TC
 * id:    ct_neg_probe_underscore_toupper_outside_domain
 * area:  ctype
 * kind:  negative
 * funcs:
 * procs: _toupper _tolower
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _toupper/_tolower (result undefined if argument not of the right case)
 * note:  PROBES that ucrtbase _toupper/_tolower are plain arithmetic (c-32 / c+32) with no domain check,
 *        so non-letters give c-32 / c+32 and no invalid-parameter call. Docs call the result undefined.
 */
#include "tc.h"
#include <ctype.h>

typedef int (__cdecl *i_i)(int);
static i_i p_up, p_lo;
static volatile int vc;
TC_CASE(toupper_arith) { vc = 'A'; CHECK_EQ_I(p_up(vc), 0x21); vc = '1'; CHECK_EQ_I(p_up(vc), 0x11); }
TC_CASE(tolower_arith) { vc = 'a'; CHECK_EQ_I(p_lo(vc), 0x81); vc = '1'; CHECK_EQ_I(p_lo(vc), 0x51); }
TC_CASE(no_iph)
{
    tc_iph_count = 0; vc = '1';
    (void)p_up(vc); (void)p_lo(vc);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_up = (i_i)tc_crt_proc("_toupper"); p_lo = (i_i)tc_crt_proc("_tolower");
    if (!p_up || !p_lo) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(toupper_arith); TC_RUN(tolower_arith); TC_RUN(no_iph);
    return tc_end();
}
