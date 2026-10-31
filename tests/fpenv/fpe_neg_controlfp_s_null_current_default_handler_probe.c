/* TC
 * id:    fpe_neg_controlfp_s_null_current_default_handler_probe
 * area:  fpenv
 * kind:  negative
 * funcs:
 * procs: _controlfp_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs Parameter validation (default handler terminates); _controlfp_s
 * note:  PROBING: _controlfp_s(NULL,0,0) with the DEFAULT invalid-parameter handler. Either NULL is tolerated (child returns 77) or
 *        the default handler kills the process (0xC0000409 fast fail / 0xC0000417). Must never hang or fail otherwise.
 */
#include "tc.h"
#include <float.h>
TC_DECL_PROC(errno_t, _controlfp_s, (unsigned int *, unsigned int, unsigned int));

TC_CASE(null_pointer_default_handler)
{
    DWORD code = 0; int ran = tc_spawn_self("nulldef", &code, 20000);
    CHECK(ran);
    CHECK(code == 77 || code == TC_STATUS_FASTFAIL || code == TC_STATUS_INVALID_CRT_PARAM);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_controlfp_s);
    if (tc_child_name()) {
        if (!tc_p__controlfp_s) return 20;
        tc_p__controlfp_s(NULL, 0, 0);
        return 77;
    }
    TC_RUN(null_pointer_default_handler);
    return tc_end();
}
