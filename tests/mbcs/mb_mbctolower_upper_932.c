/* TC
 * id:    mb_mbctolower_upper_932
 * area:  mbcs
 * funcs: _mbctolower _mbctoupper _setmbcp _getmbcp
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _mbctolower, _mbctoupper (single-byte)
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>
static int old_cp;
static void fill(unsigned char *p, int n) { int i; for (i = 0; i < n; i++) p[i] = 0xFF; }
TC_CASE(cp) { CHECK_EQ_I(_setmbcp(932), 0); }
TC_CASE(sbcs)
{
    CHECK_EQ_U(_mbctolower('A'), 'a'); CHECK_EQ_U(_mbctoupper('z'), 'Z');
    CHECK_EQ_U(_mbctolower('1'), '1'); CHECK_EQ_U(_mbctoupper('1'), '1');
}
int main(void) { tc_begin(); old_cp = _getmbcp(); TC_RUN(cp); TC_RUN(sbcs); _setmbcp(old_cp); return tc_end(); }
