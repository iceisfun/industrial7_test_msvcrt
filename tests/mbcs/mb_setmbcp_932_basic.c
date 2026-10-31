/* TC
 * id:    mb_setmbcp_932_basic
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _setmbcp, _getmbcp
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

static volatile int v_cp;
TC_CASE(set_932)
{
    int saved = _getmbcp();
    v_cp = 932;
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_getmbcp(), 932);
    v_cp = saved;
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_getmbcp(), saved);
}
TC_CASE(set_932_twice)
{
    int saved = _getmbcp();
    v_cp = 932;
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_setmbcp(v_cp), 0);
    CHECK_EQ_I(_getmbcp(), 932);
    _setmbcp(saved);
}

int main(void) { tc_begin(); TC_RUN(set_932); TC_RUN(set_932_twice); return tc_end(); }
