/* TC
 * id:    gsp_neg_getpid_not_exported_probe
 * area:  process
 * funcs:
 * procs: getpid _getpid
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _getpid; oldnames.lib supplies getpid as a static thunk to _getpid
 * note:  PROBE: no CRT DLL exports the unprefixed getpid (the POSIX name comes from the static oldnames thunk to _getpid). Believed from the build: unprefixed calls link to a thunk importing _getpid from the DLL. The _getpid assertion is its own case.
 */
#include "tc.h"

TC_CASE(underscore_exported) { CHECK(tc_crt_proc("_getpid") != NULL); }
TC_CASE(plain_not_exported) { CHECK(tc_crt_proc("getpid") == NULL); }
int main(void) { tc_begin(); TC_RUN(underscore_exported); TC_RUN(plain_not_exported); return tc_end(); }
