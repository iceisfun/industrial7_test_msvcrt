/* TC
 * id:    exp_probe_hypot_status87_exports
 * area:  exports
 * funcs:
 * procs: _hypot _hypotf hypot hypotf _status87
 * crt:   msvcr120 ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs _hypot, hypot, hypotf, _status87
 * note:  Probing three independent guesses, one case each so a wrong guess cannot mask the others:
 *        (1) hypot is a real export (not just a header inline over _hypot);
 *        (2) hypotf is a real export (it may exist only as _hypotf or as a header inline);
 *        (3) _status87 is exported on x64 (it is documented for x86; _statusfp is the portable name).
 * note:  RESULT (Win10 22H2 x64 19045 ucrtbase export table, dumped in the reference VM): _hypot, hypot and _hypotf are exported; hypotf and _status87 are
 *        NOT (hypotf is only a header inline; _status87 is x86-only, _statusfp is the x64 name). Cases (2) and (3) therefore assert absence. Build-dependent: a
 *        future ucrtbase could add exports.
 */
#include "tc.h"
#define HAS(n) do { if (!tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "not exported: " n); } while (0)
#define LACKS(n) do { if (tc_crt_proc(n)) tc_fail_expr(__FILE__, __LINE__, "unexpectedly exported: " n); } while (0)

TC_CASE(hypot_double) { HAS("_hypot"); HAS("hypot"); }
TC_CASE(hypot_float)  { LACKS("hypotf"); }   /* class D: ucrtbase 10.0.19041 exports only _hypotf (hypotf is a header inline) */
TC_CASE(underscore_hypotf) { HAS("_hypotf"); }
TC_CASE(status87)     { LACKS("_status87"); HAS("_statusfp"); }   /* class D: x86-only name; x64 ucrtbase exports _statusfp */

int main(void)
{
    tc_begin();
    TC_RUN(hypot_double); TC_RUN(hypot_float); TC_RUN(underscore_hypotf); TC_RUN(status87);
    return tc_end();
}
