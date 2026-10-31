/* TC
 * id:    prc_neg_system_unknown_command_probe
 * area:  process
 * kind:  negative
 * funcs: system
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   cmd.exe /c behaviour (not documented by the CRT)
 * note:  Probes cmd.exe's exit status for (a) an unrecognised command (expected nonzero, most likely 1; 9009 also accepted) and (b) an empty command line
 *        (expected 0). These depend on cmd.exe, not the CRT.
 */
#include "tc.h"
#include <stdlib.h>

static int run(const char *c) { const char *volatile v = c; return system(v); }

TC_CASE(unknown_command) { { int r = run("tc_no_such_cmd_zz 2>nul"); CHECK(r != 0); CHECK(r == 1 || r == 9009); } }
TC_CASE(empty_command)   { CHECK_EQ_I(run(""), 0); }

int main(void) { tc_begin(); TC_RUN(unknown_command); TC_RUN(empty_command); return tc_end(); }
