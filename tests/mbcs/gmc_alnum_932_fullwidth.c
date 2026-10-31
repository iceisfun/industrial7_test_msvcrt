/* TC
 * id:    gmc_alnum_932_fullwidth
 * area:  mbcs
 * funcs: _ismbcalnum _setmbcp
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _ismbcalnum
 * note:  PROBE: fullwidth Latin/digits (JIS X 0208 row 3) are alnum, ideographic space/comma are not, under cp932.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>

static volatile unsigned int v;
static int mb(unsigned int c, int (__cdecl *f)(unsigned int)) { v = c; return f(v) != 0; }
#define A(c) mb(c, _ismbcalnum)
TC_CASE(fw_alpha) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(A(0x8260)); CHECK(A(0x8281)); }
TC_CASE(fw_digit) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(A(0x824F)); }
TC_CASE(fw_notalnum) { CHECK_EQ_I(_setmbcp(932), 0); CHECK(!A(0x8140)); CHECK(!A(0x8141)); }
int main(void) { tc_begin(); TC_RUN(fw_alpha); TC_RUN(fw_digit); TC_RUN(fw_notalnum); _setmbcp(-3); return tc_end(); }
