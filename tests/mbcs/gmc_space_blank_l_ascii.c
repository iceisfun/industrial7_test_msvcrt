/* TC
 * id:    gmc_space_blank_l_ascii
 * area:  mbcs
 * funcs: _ismbcspace_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcspace; C11 7.4.1.10 isspace
 * note:  ASCII whitespace only; ideographic space in gmc_space_l_ideographic_probe.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
static int sl(unsigned c) { v = c; return _ismbcspace_l(v, NULL) != 0; }
TC_CASE(white) { unsigned c; CHECK_EQ_I(_setmbcp(932), 0); CHECK(sl(0x20)); for (c = 9; c <= 13; c++) CHECK(sl(c)); }
TC_CASE(nonwhite) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!sl('a')); CHECK(!sl('0')); CHECK(!sl(0x1F)); CHECK(!sl(0x7F)); }
int main(void) { tc_begin(); TC_RUN(white); TC_RUN(nonwhite); _setmbcp(-3); return tc_end(); }
