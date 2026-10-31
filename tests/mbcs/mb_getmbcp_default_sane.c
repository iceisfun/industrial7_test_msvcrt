/* TC
 * id:    mb_getmbcp_default_sane
 * area:  mbcs
 * funcs: _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   MS docs _getmbcp
 * note:  only sanity bounds, no exact value
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

TC_CASE(bounds) { int cp = _getmbcp(); CHECK(cp >= 0 && cp < 65536); }
TC_CASE(known_value)
{
    int cp = _getmbcp();
    CHECK(cp == 0 || cp == (int)GetACP() || cp == (int)GetOEMCP() || IsValidCodePage((UINT)cp));
}
TC_CASE(stable) { int a = _getmbcp(), b = _getmbcp(); CHECK_EQ_I(a, b); }

int main(void) { tc_begin(); TC_RUN(bounds); TC_RUN(known_value); TC_RUN(stable); return tc_end(); }
