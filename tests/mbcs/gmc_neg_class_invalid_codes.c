/* TC
 * id:    gmc_neg_class_invalid_codes
 * area:  mbcs
 * funcs: _ismbcalnum _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  inferred
 * ref:   MS docs _ismbcalnum etc.
 * note:  Lone lead byte, 0 and 0xFF return 0 (invalid-trail pairs are in gmc_neg_class_bad_trail_probe); no errno assertion; invalid-parameter handler must not fire where supported.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
static void none(unsigned c) { CHECK(!mb(c, _ismbcalnum)); }
TC_CASE(lead_alone) { CHECK_EQ_I(_setmbcp(932), 0); none(0x82); }
TC_CASE(zero) { CHECK_EQ_I(_setmbcp(932), 0); none(0); }
TC_CASE(ff) { CHECK_EQ_I(_setmbcp(932), 0); none(0xFF); }
TC_CASE(no_iph) { CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0; none(0x82); none(0xFF); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(lead_alone); TC_RUN(zero); TC_RUN(ff); TC_RUN(no_iph); _setmbcp(-3); return tc_end(); }
