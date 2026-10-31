/* TC
 * id:    exp_probe_hypot_status87_exports
 * area:  exports
 * funcs:
 * procs: _hypot _hypotf hypot hypotf _status87
 * crt:   msvcr120 ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _hypot, hypot, hypotf, _status87
 * note:  Probing three independent guesses, one case each so a wrong guess cannot mask the others:
 *        (1) hypot is a real export (not just a header inline over _hypot);
 *        (2) hypotf is a real export (it may exist only as _hypotf or as a header inline);
 *        (3) _status87 is exported on x64 (it is documented for x86; _statusfp is the portable name).
 */
#include "tc.h"
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)

TC_CASE(hypot_double) { HAS("_hypot"); HAS("hypot"); }
TC_CASE(hypot_float)  { HAS("hypotf"); }
TC_CASE(underscore_hypotf) { HAS("_hypotf"); }
TC_CASE(status87)     { HAS("_status87"); }

int main(void)
{
    tc_begin();
    TC_RUN(hypot_double); TC_RUN(hypot_float); TC_RUN(underscore_hypotf); TC_RUN(status87);
    return tc_end();
}
