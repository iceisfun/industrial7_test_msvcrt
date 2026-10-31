/* TC
 * id:    gmc_neg_class_bad_trail_probe
 * area:  mbcs
 * funcs: _ismbcalnum _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ismbcalnum
 * note:  PROBE: double-byte values with an invalid cp932 trail byte (0x8200, 0x8120) are not alnum and do not
 *        call the invalid-parameter handler.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(bad_trail) { CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0; CHECK(!mb(0x8200, _ismbcalnum)); CHECK(!mb(0x8120, _ismbcalnum)); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(bad_trail); _setmbcp(-3); return tc_end(); }
