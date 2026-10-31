/* TC
 * id:    gmc_class_l_cp1252_ascii
 * area:  mbcs
 * funcs: _ismbcalnum_l _ismbcpunct_l _ismbcgraph_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcalnum, _ismbcpunct, _ismbcgraph
 * note:  ASCII classification under single-byte code page 1252 with NULL locale. Restores _setmbcp(-3).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int an(unsigned c) { v = c; return _ismbcalnum_l(v, NULL) != 0; }
static int pu(unsigned c) { v = c; return _ismbcpunct_l(v, NULL) != 0; }
static int gr(unsigned c) { v = c; return _ismbcgraph_l(v, NULL) != 0; }
TC_CASE(ascii_ok)
{
    CHECK_EQ_I(_setmbcp(1252), 0);
    CHECK(an('A')); CHECK(an('z')); CHECK(an('5')); CHECK(!an('!')); CHECK(!an(' '));
    CHECK(pu('!')); CHECK(pu('/')); CHECK(!pu('a')); CHECK(!pu(' '));
    CHECK(gr('A')); CHECK(gr('~')); CHECK(!gr(' ')); CHECK(!gr(0x1F));
}
int main(void)
{
    tc_begin();
    TC_RUN(ascii_ok);
    _setmbcp(-3);
    return tc_end();
}
