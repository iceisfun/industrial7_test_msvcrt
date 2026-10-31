/* TC
 * id:    gmc_graph_print_l_basic
 * area:  mbcs
 * funcs: _ismbcgraph _ismbcgraph_l _ismbcprint _ismbcprint_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcgraph, _ismbcprint
 * note:  NULL locale = current. 0x82A0 split into gmc_graph_print_l_dbcs_probe.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
static int gl(unsigned c) { v = c; return _ismbcgraph_l(v, NULL) != 0; }
static int pl(unsigned c) { v = c; return _ismbcprint_l(v, NULL) != 0; }
TC_CASE(graph) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(gl('A')); CHECK(!gl(' ')); CHECK(!gl(0x1F)); }
TC_CASE(print) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(pl('A')); CHECK(pl(' ')); CHECK(!pl(0x1F)); CHECK(!pl(0x7F)); }
TC_CASE(agree) { unsigned c; CHECK_EQ_I(_setmbcp(932), 0); for (c = 0; c < 0x80; c++) { CHECK_EQ_I(gl(c), mb(c, _ismbcgraph)); CHECK_EQ_I(pl(c), mb(c, _ismbcprint)); } }
int main(void) { tc_begin(); TC_RUN(graph); TC_RUN(print); TC_RUN(agree); _setmbcp(-3); return tc_end(); }
