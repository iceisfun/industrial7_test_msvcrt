/* TC
 * id:    prc_neg_system_unknown_exit_distinct_probe
 * area:  process
 * funcs: system _wsystem
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs system (returns the command interpreter's exit code)
 * note:  PROBE: exit codes above 255 are returned unmasked as int (cmd.exe passes a full DWORD exit code). The 2147483648 case
 *        additionally assumes cmd.exe saturates it (strtol-style) to 0x7FFFFFFF; it is separate for that reason.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(sys_256)     { CHECK_EQ_I(system("exit 256"), 256); }
TC_CASE(wsys_1000)   { CHECK_EQ_I(_wsystem(L"exit 1000"), 1000); }
TC_CASE(sys_2p31)   { CHECK_EQ_I(system("exit 2147483648"), 2147483647); }
int main(void) { tc_begin(); TC_RUN(sys_256); TC_RUN(wsys_1000); TC_RUN(sys_2p31); return tc_end(); }
