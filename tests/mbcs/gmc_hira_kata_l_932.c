/* TC
 * id:    gmc_hira_kata_l_932
 * area:  mbcs
 * funcs: _ismbchira_l _ismbckata_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbchira (0x829F-0x82F1), _ismbckata (0x8340-0x8396)
 * note:  0x837F hole and halfwidth 0xB1 are in gmc_hira_kata_l_932_edge_probe.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
static int hl(unsigned c) { v = c; return _ismbchira_l(v, NULL) != 0; }
static int kl(unsigned c) { v = c; return _ismbckata_l(v, NULL) != 0; }
TC_CASE(hira_in) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(hl(0x829F)); CHECK(hl(0x82A0)); CHECK(hl(0x82F1)); }
TC_CASE(hira_out) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!hl(0x829E)); CHECK(!hl(0x82F2)); CHECK(!hl(0x8340)); }
TC_CASE(kata_in) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(kl(0x8340)); CHECK(kl(0x8341)); CHECK(kl(0x8396)); }
TC_CASE(kata_out) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!kl(0x8397)); CHECK(!kl(0x829F)); }
int main(void) { tc_begin(); TC_RUN(hira_in); TC_RUN(hira_out); TC_RUN(kata_in); TC_RUN(kata_out); _setmbcp(-3); return tc_end(); }
