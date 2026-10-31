/* TC
 * id:    gmc_neg_class_l_wrong_codepage
 * area:  mbcs
 * funcs: _ismbcalnum_l _ismbcpunct_l _ismbcgraph_l _setmbcp
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ismbcalnum, _ismbcpunct, _ismbcgraph, _setmbcp
 * note:  PROBE: Under single-byte code page 1252 there are no lead bytes, so double-byte codes are not valid characters and classify false (CRT-internal handling of non-lead high bytes is undocumented). ASCII checks live in gmc_class_l_cp1252_ascii. Restores _setmbcp(-3).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int an(unsigned c) { v = c; return _ismbcalnum_l(v, NULL) != 0; }
static int pu(unsigned c) { v = c; return _ismbcpunct_l(v, NULL) != 0; }
static int gr(unsigned c) { v = c; return _ismbcgraph_l(v, NULL) != 0; }
TC_CASE(alnum_dbcs) { CHECK_EQ_I(_setmbcp(1252), 0); CHECK(!an(0x8260)); CHECK(!an(0x82A0)); }
TC_CASE(punct_dbcs) { CHECK_EQ_I(_setmbcp(1252), 0); CHECK(!pu(0x8141)); CHECK(!pu(0x8142)); }
TC_CASE(graph_dbcs) { CHECK_EQ_I(_setmbcp(1252), 0); CHECK(!gr(0x889F)); CHECK(!gr(0x8140)); }
int main(void)
{
    tc_begin();
    TC_RUN(alnum_dbcs); TC_RUN(punct_dbcs); TC_RUN(graph_dbcs);
    _setmbcp(-3);
    return tc_end();
}
