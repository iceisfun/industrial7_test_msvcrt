/* TC
 * id:    gsj_probe_query_app_type_console
 * area:  exit_signal
 * funcs:
 * procs: _query_app_type
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented; _crt_app_type enum: unknown=0, console=1, gui=2)
 * note:  PROBING that a console exe started via the mingw startup (which calls __set_app_type(console)) reports 1.
 *        Depends on the corpus toolchain startup code, not only on the CRT.
 */
#include "tc.h"

TC_DECL_PROC(int, _query_app_type, (void));

TC_CASE(console_is_one)
{
    if (!tc_p__query_app_type) TC_SKIP("missing");
    CHECK_EQ_I(tc_p__query_app_type(), 1);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_query_app_type);
    TC_RUN(console_is_one);
    return tc_end();
}
