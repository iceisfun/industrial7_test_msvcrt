/* TC
 * id:    gmc_neg_blank_a0_probe
 * area:  mbcs
 * funcs: _ismbcblank _setmbcp
 * crt:   ucrtbase msvcr120
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ismbcblank
 * note:  PROBE: under cp932 the byte 0xA0 (undefined in cp932; no-break space in cp1252) is not a blank.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int b(unsigned c) { v = c; return _ismbcblank(v) != 0; }
TC_CASE(a0) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!b(0xA0)); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(a0); _setmbcp(-3); return tc_end(); }
