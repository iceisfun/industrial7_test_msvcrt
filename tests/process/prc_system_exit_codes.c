/* TC
 * id:    prc_system_exit_codes
 * area:  process
 * funcs: system
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs system: returns the command interpreter's return value
 */
#include "tc.h"
#include <stdlib.h>

static int run(const char *c) { const char *volatile v = c; return system(v); }

TC_CASE(exit0)   { CHECK_EQ_I(run("exit 0"), 0); }
TC_CASE(exit3)   { CHECK_EQ_I(run("exit 3"), 3); }
TC_CASE(exit255) { CHECK_EQ_I(run("exit 255"), 255); }
TC_CASE(echo_nul){ CHECK_EQ_I(run("echo x>nul"), 0); }

int main(void) { tc_begin(); TC_RUN(exit0); TC_RUN(exit3); TC_RUN(exit255); TC_RUN(echo_nul); return tc_end(); }
