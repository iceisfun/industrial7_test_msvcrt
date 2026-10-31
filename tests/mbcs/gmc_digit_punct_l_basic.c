/* TC
 * id:    gmc_digit_punct_l_basic
 * area:  mbcs
 * funcs: _ismbcdigit_l _ismbcpunct_l _setmbcp
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcdigit, _ismbcpunct
 * note:  ASCII range is documented; fullwidth zero is a separate probe case in a separate probe test.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
static int dl(unsigned c) { v = c; return _ismbcdigit_l(v, NULL) != 0; }
static int pl(unsigned c) { v = c; return _ismbcpunct_l(v, NULL) != 0; }
TC_CASE(digit) { unsigned c; CHECK_EQ_I(_setmbcp(932), 0); for (c = '0'; c <= '9'; c++) CHECK(dl(c)); CHECK(!dl('a')); CHECK(!dl('/')); CHECK(!dl(':')); }
TC_CASE(punct) { unsigned i; const unsigned p[] = {'!','/',':','@','[','`','{','~'}; CHECK_EQ_I(_setmbcp(932), 0); for (i = 0; i < 8; i++) CHECK(pl(p[i])); }
TC_CASE(not_punct) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!pl('a')); CHECK(!pl('5')); CHECK(!pl(' ')); }
int main(void) { tc_begin(); TC_RUN(digit); TC_RUN(punct); TC_RUN(not_punct); _setmbcp(-3); return tc_end(); }
