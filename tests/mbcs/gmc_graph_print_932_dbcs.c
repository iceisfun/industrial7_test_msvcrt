/* TC
 * id:    gmc_graph_print_932_dbcs
 * area:  mbcs
 * funcs: _ismbcgraph _ismbcprint _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcgraph, _ismbcprint
 * note:  PROBE: JIS X 0208 chars, ideographic space and half-width katakana are printable under cp932; graph of 0x8140 not asserted.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(hira) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0x82A0, _ismbcgraph)); CHECK(mb(0x82A0, _ismbcprint)); }
TC_CASE(kanji) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0x889F, _ismbcgraph)); CHECK(mb(0x889F, _ismbcprint)); }
TC_CASE(ideo_space) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0x8140, _ismbcprint)); }
TC_CASE(halfwidth_kana) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0xB1, _ismbcgraph)); CHECK(mb(0xB1, _ismbcprint)); }
int main(void) { tc_begin(); TC_RUN(hira); TC_RUN(kanji); TC_RUN(ideo_space); TC_RUN(halfwidth_kana); _setmbcp(-3); return tc_end(); }
