/* TC
 * id:    gsj_probe_query_app_type
 * area:  exit_signal
 * kind:  positive
 * funcs:
 * procs: _query_app_type
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   none (undocumented; _crt_app_type enum: unknown=0, console=1, gui=2)
 * note:  PROBING _query_app_type. Only export presence and a 0..2 range. The console==1 guess lives in
 *        gsj_probe_query_app_type_console.
 */
#include "tc.h"

TC_DECL_PROC(int, _query_app_type, (void));

TC_CASE(exported) { CHECK(tc_p__query_app_type != NULL); }
TC_CASE(in_range)
{
    int r; if (!tc_p__query_app_type) TC_SKIP("missing");
    r = tc_p__query_app_type();
    CHECK(r >= 0 && r <= 2);
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(_query_app_type);
    TC_RUN(exported); TC_RUN(in_range);
    return tc_end();
}
