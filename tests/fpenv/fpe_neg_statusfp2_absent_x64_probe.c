/* TC
 * id:    fpe_neg_statusfp2_absent_x64_probe
 * area:  fpenv
 * kind:  negative
 * funcs: _controlfp
 * procs: _statusfp2 _status87
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _statusfp2 (x86 only, "Requirements")
 * note:  PROBES whether the x86-only exports _statusfp2 / _status87 are present on x64. Presence is only logged as TC INFO;
 *        nothing is asserted about it and the functions are never called.
 */
#include "tc.h"
#include <float.h>

static unsigned cw(void) { return _controlfp(0, 0); }
static void info(const char *name, FARPROC p)
{
    tc_write("TC INFO " TC_TEST_ID "::"); tc_write(name); tc_write(p ? " present\n" : " absent\n");
}
TC_CASE(statusfp2_presence) { info("_statusfp2", tc_crt_proc("_statusfp2")); CHECK(cw() != 0xffffffffu); }
TC_CASE(status87_presence) { info("_status87", tc_crt_proc("_status87")); CHECK(cw() != 0xffffffffu); }
int main(void)
{
    tc_begin();
    TC_RUN(statusfp2_presence); TC_RUN(status87_presence);
    return tc_end();
}
