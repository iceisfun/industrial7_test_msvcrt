/* TC
 * id:    prc_neg_system_unknown_exit_distinct_probe
 * area:  process
 * funcs: system _wsystem
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs system (returns the command interpreter's exit code)
 * note:  Observed on Win10 22H2 19045 (cmd.exe + ucrtbase system): exit codes above 255 are returned unmasked as int (sys_256,
 *        wsys_1000 as before) and "exit 2147483648" yields (int)0x80000000 = INT_MIN; cmd.exe does NOT saturate to 0x7FFFFFFF
 *        as the old guess assumed. cmd.exe-version dependent (not ucrt).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(sys_256)     { CHECK_EQ_I(system("exit 256"), 256); }
TC_CASE(wsys_1000)   { CHECK_EQ_I(_wsystem(L"exit 1000"), 1000); }
TC_CASE(sys_2p31)   { CHECK_EQ_I(system("exit 2147483648"), (-2147483647 - 1)); }
int main(void) { tc_begin(); TC_RUN(sys_256); TC_RUN(wsys_1000); TC_RUN(sys_2p31); return tc_end(); }
