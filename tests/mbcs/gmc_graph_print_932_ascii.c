/* TC
 * id:    gmc_graph_print_932_ascii
 * area:  mbcs
 * funcs: _ismbcgraph _ismbcprint _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcgraph, _ismbcprint; C11 7.4.1.6/7.4.1.8
 * note:  ASCII range only.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(graphic) { const unsigned s[] = {'A','!','~','a'}; unsigned i; CHECK_EQ_I(_setmbcp(932), 0); for (i = 0; i < 4; i++) { CHECK(mb(s[i], _ismbcgraph)); CHECK(mb(s[i], _ismbcprint)); } }
TC_CASE(space) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0x20, _ismbcprint)); CHECK(!mb(0x20, _ismbcgraph)); }
TC_CASE(control) { const unsigned s[] = {0, 0x1F, 0x7F}; unsigned i; CHECK_EQ_I(_setmbcp(932), 0); for (i = 0; i < 3; i++) { CHECK(!mb(s[i], _ismbcgraph)); CHECK(!mb(s[i], _ismbcprint)); } }
TC_CASE(implies) { unsigned c; CHECK_EQ_I(_setmbcp(932), 0); for (c = 0; c < 0x80; c++) if (mb(c, _ismbcgraph)) CHECK(mb(c, _ismbcprint)); }
int main(void) { tc_begin(); TC_RUN(graphic); TC_RUN(space); TC_RUN(control); TC_RUN(implies); _setmbcp(-3); return tc_end(); }
