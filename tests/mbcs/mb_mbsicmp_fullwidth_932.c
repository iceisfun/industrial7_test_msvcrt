/* TC
 * id:    mb_mbsicmp_fullwidth_932
 * area:  mbcs
 * funcs: _mbsicmp _mbscmp _setmbcp
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _mbsicmp (case folding); CRT mbcs tables
 * note:  PROBE: in cp932 _mbsicmp folds full-width Latin capitals 0x8260-0x8279 to 0x8281-0x829A (JIS X 0208 fullwidth A..Z / a..z).
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

TC_CASE(fold_equal)
{
    static unsigned char a[] = { 0x82, 0x60, 0 }, b[] = { 0x82, 0x81, 0 };
    CHECK_EQ_I(_mbsicmp(a, b), 0); CHECK(_mbscmp(a, b) != 0);
}
TC_CASE(fold_order)
{
    static unsigned char a[] = { 0x82, 0x60, 0 }, b[] = { 0x82, 0x82, 0 };
    CHECK(_mbsicmp(a, b) < 0);
}

int main(void)
{
    int old = _getmbcp();
    tc_begin(); _setmbcp(932);
    TC_RUN(fold_equal); TC_RUN(fold_order);
    _setmbcp(old); return tc_end();
}
