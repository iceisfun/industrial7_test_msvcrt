/* TC
 * id:    err_neg_noinfo_noreturn_terminates
 * area:  errno_param
 * funcs: _invalid_parameter_noinfo_noreturn
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs: _invalid_parameter_noinfo_noreturn does not return
 * note:  Only the no-handler path is asserted (documented as non-returning). What happens after a user handler RETURNS is not documented
 *        and differs by CRT generation; it is covered by the separate probe tests (see tests/negative_fatal/nfx_neg_iph_noreturn_handler_returns).
 */
#include "tc.h"
#include <stdlib.h>

extern void __cdecl _invalid_parameter_noinfo_noreturn(void);

TC_CASE(no_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("nohandler", &code, 20000);
    CHECK(ran); CHECK(code != 0); CHECK(code != 77);
}

int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        _invalid_parameter_noinfo_noreturn();
        return 77;
    }
    TC_RUN(no_handler);
    return tc_end();
}
