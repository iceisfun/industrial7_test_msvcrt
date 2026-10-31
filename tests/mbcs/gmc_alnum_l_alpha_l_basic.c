/* TC
 * id:    gmc_alnum_l_alpha_l_basic
 * area:  mbcs
 * funcs: _ismbcalnum _ismbcalnum_l _ismbcalpha _ismbcalpha_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcalnum, _ismbcalpha, _l variants
 * note:  NULL locale means current locale. Fullwidth case moved to gmc_alnum_l_alpha_fullwidth_probe. msvcr80/msvcrt excluded (no _l exports there).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
#define AN(c) mb(c, _ismbcalnum)
static int anl(unsigned c) { v = c; return _ismbcalnum_l(v, NULL) != 0; }
static int apl(unsigned c) { v = c; return _ismbcalpha_l(v, NULL) != 0; }
TC_CASE(alnum_l) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(anl('a')); CHECK(anl('7')); CHECK(!anl('#')); }
TC_CASE(alpha_l) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(apl('a')); CHECK(apl('Z')); CHECK(!apl('7')); CHECK(!apl(' ')); }
TC_CASE(agree) { unsigned c; CHECK_EQ_I(_setmbcp(932), 0);
    for (c = 0; c < 0x80; c++) { CHECK_EQ_I(anl(c), AN(c)); CHECK_EQ_I(apl(c), mb(c, _ismbcalpha)); } }
int main(void) { tc_begin(); TC_RUN(alnum_l); TC_RUN(alpha_l); TC_RUN(agree); _setmbcp(-3); return tc_end(); }
