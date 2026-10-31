/* TC
 * id:    nfx_neg_iph_noinfo_direct
 * area:  negative_fatal
 * kind:  negative
 * funcs: _invalid_parameter_noinfo _set_invalid_parameter_handler
 * procs: 
 * crt:   >=90
 * tier:  p1
 * conf:  observed
 * ref:   used by MS inline/secure CRT code; not publicly documented as an API
 * note:  Direct call invokes the current handler (counting handler returns), and with the default handler terminates.
 */
#include "tc.h"
#include <stdlib.h>
void __cdecl _invalid_parameter_noinfo(void);
TC_CASE(counting)
{
    tc_iph_count = 0;
    CHECK(tc_iph_install());
    _invalid_parameter_noinfo();
    CHECK_EQ_I(tc_iph_count, 1);
    _set_invalid_parameter_handler(NULL);
}
TC_CASE(default_fatal) { DWORD c = 0; CHECK(tc_spawn_self("direct", &c, 20000)); CHECK(c != 0); CHECK(c != 77); }
int main(void)
{
    tc_begin();
    if (tc_child_name()) { _invalid_parameter_noinfo(); return 77; }
    TC_RUN(counting); TC_RUN(default_fatal);
    return tc_end();
}
