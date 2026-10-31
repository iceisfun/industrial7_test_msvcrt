/* TC
 * id:    gmc_graph_print_l_dbcs_probe
 * area:  mbcs
 * funcs: _ismbcgraph_l _ismbcprint_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcgraph
 * note:  PROBE: hiragana 0x82A0 is graph and print via _l under cp932.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
TC_CASE(hira) { CHECK_EQ_I(_setmbcp(932), 0); v = 0x82A0; CHECK(_ismbcgraph_l(v, NULL) != 0); CHECK(_ismbcprint_l(v, NULL) != 0); }
int main(void) { tc_begin(); TC_RUN(hira); _setmbcp(-3); return tc_end(); }
