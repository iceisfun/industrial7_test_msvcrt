/* TC
 * id:    prc_neg_popen_unknown_command_probe
 * area:  process
 * funcs: _popen _pclose
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _popen
 * note:  PROBE: a nonexistent command run via cmd.exe makes _popen succeed (interpreter starts) and failure shows as nonzero _pclose status.
 */
#include "tc.h"
#include <stdio.h>
TC_CASE(unknown_cmd)
{
    int r;
    FILE *volatile f = _popen("tc_no_such_cmd_zz 2>nul", "r");
    CHECK(f != NULL);
    if (!f) return;
    r = _pclose(f);
    CHECK(r != 0);
}
int main(void) { tc_begin(); TC_RUN(unknown_cmd); return tc_end(); }
