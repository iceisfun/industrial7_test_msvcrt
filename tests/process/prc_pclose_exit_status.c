/* TC
 * id:    prc_pclose_exit_status
 * area:  process
 * funcs: _popen _pclose
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _pclose: returns the exit status of the command
 */
#include "tc.h"
#include <stdio.h>
static int run(const char *cmd)
{
    FILE *volatile f = _popen(cmd, "r");
    if (!f) return -1000;
    return _pclose(f);
}
TC_CASE(exit6)   { CHECK_EQ_I(run("cmd /c exit 6"), 6); }
TC_CASE(exit0)   { CHECK_EQ_I(run("cmd /c exit 0"), 0); }
TC_CASE(exit255) { CHECK_EQ_I(run("cmd /c exit 255"), 255); }
int main(void) { tc_begin(); TC_RUN(exit6); TC_RUN(exit0); TC_RUN(exit255); return tc_end(); }
