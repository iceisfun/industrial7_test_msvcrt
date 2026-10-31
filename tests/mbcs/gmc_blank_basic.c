/* TC
 * id:    gmc_blank_basic
 * area:  mbcs
 * funcs: _setmbcp
 * procs: _ismbcblank _ismbcblank_l
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcblank (VS2013+): space or tab
 * note:  _ismbcblank absent from msvcr110 and earlier. 0x8140 handled in gmc_blank_ideographic_probe.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

TC_DECL_PROC(int, _ismbcblank, (unsigned int));
TC_DECL_PROC(int, _ismbcblank_l, (unsigned int, _locale_t));
static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
static int bl(unsigned c) { v = c; return tc_p__ismbcblank_l(v, NULL) != 0; }
TC_CASE(blank) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(mb(0x20, tc_p__ismbcblank)); CHECK(mb(0x09, tc_p__ismbcblank)); }
TC_CASE(notblank) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!mb('a', tc_p__ismbcblank)); CHECK(!mb('0', tc_p__ismbcblank)); CHECK(!mb(0x0A, tc_p__ismbcblank)); CHECK(!mb(0x0D, tc_p__ismbcblank)); }
TC_CASE(l_agrees) { unsigned c; CHECK_EQ_I(_setmbcp(932), 0); for (c = 0; c < 0x80; c++) CHECK_EQ_I(bl(c), mb(c, tc_p__ismbcblank)); }
int main(void) { tc_begin(); TC_LOAD_PROC(_ismbcblank); TC_LOAD_PROC(_ismbcblank_l); CHECK(tc_p__ismbcblank && tc_p__ismbcblank_l); TC_RUN(blank); TC_RUN(notblank); TC_RUN(l_agrees); _setmbcp(-3); return tc_end(); }
