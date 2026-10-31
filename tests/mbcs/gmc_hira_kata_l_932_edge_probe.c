/* TC
 * id:    gmc_hira_kata_l_932_edge_probe
 * area:  mbcs
 * funcs: _ismbckata_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbckata
 * note:  PROBE: 0x837F (not a valid cp932 character, inside the documented 0x8340-0x8396 span) is not katakana;
 *        halfwidth katakana single byte 0xB1 is not _ismbckata (docs: full-width katakana only).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int kl(unsigned c) { v = c; return _ismbckata_l(v, NULL) != 0; }
TC_CASE(hole) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!kl(0x837F)); }
TC_CASE(halfwidth) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!kl(0xB1)); }
int main(void) { tc_begin(); TC_RUN(hole); TC_RUN(halfwidth); _setmbcp(-3); return tc_end(); }
