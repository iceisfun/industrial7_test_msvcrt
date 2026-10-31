/* TC
 * id:    gmc_alnum_932_ascii
 * area:  mbcs
 * funcs: _ismbcalnum _setmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _ismbcalnum
 * note:  Single-byte only; truthiness only. Restores _setmbcp(-3).
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
#define A(c) mb(c, _ismbcalnum)
TC_CASE(letters) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(A('a')); CHECK(A('z')); CHECK(A('A')); CHECK(A('Z')); }
TC_CASE(digits) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(A('0')); CHECK(A('9')); }
TC_CASE(punct) { unsigned c; const unsigned p[] = {'!','/',':','@','[','`','{','~'}; CHECK_EQ_I(_setmbcp(932), 0); for (c = 0; c < 8; c++) CHECK(!A(p[c])); }
TC_CASE(space_nul) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!A(' ')); CHECK(!A('\t')); CHECK(!A(0)); }
int main(void) { tc_begin(); TC_RUN(letters); TC_RUN(digits); TC_RUN(punct); TC_RUN(space_nul); _setmbcp(-3); return tc_end(); }
