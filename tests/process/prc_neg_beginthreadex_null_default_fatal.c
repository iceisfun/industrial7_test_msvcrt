/* TC
 * id:    prc_neg_beginthreadex_null_default_fatal
 * area:  process
 * funcs: _beginthreadex
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _beginthread/_beginthreadex: NULL start address is an invalid parameter; default handler terminates
 */
#include "tc.h"
#include <process.h>

TC_CASE(child_terminates)
{
    DWORD code = 0; CHECK(tc_spawn_self("bt", &code, 20000));
    CHECK(code != 0);
    CHECK(code != 77);
}

int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        tc_iph_default();
        _beginthreadex(NULL, 0, NULL, NULL, 0, NULL);
        return 77;
    }
    TC_RUN(child_terminates);
    return tc_end();
}
