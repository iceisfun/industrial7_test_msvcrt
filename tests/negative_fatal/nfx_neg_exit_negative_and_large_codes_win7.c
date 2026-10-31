/* TC
 * id:    nfx_neg_exit_negative_and_large_codes_win7
 * area:  negative_fatal
 * kind:  negative
 * funcs: exit _exit
 * crt:   win7
 * tier:  p1
 * conf:  probe
 * ref:   MS docs exit, _exit (status is passed to ExitProcess); C11 7.22.4.4 (only 0/EXIT_* are portable)
 * note:  Probes: the CRT passes the full 32-bit status to ExitProcess, so exit(-1) yields 0xFFFFFFFF, exit(256) yields 256 (no 8-bit
 *        truncation as on POSIX), and _exit(0x12345678) yields 0x12345678.
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(exit_minus_one)   { DWORD c = 0; CHECK(tc_spawn_self("m1", &c, 20000));  CHECK_EQ_U(c, 0xFFFFFFFFu); }
TC_CASE(exit_256)         { DWORD c = 0; CHECK(tc_spawn_self("e256", &c, 20000)); CHECK_EQ_U(c, 256); }
TC_CASE(underscore_exit_wide) { DWORD c = 0; CHECK(tc_spawn_self("wide", &c, 20000)); CHECK_EQ_U(c, 0x12345678u); }
int main(void)
{
    const char *n; tc_begin();
    if ((n = tc_child_name()) != NULL) {
        volatile int v;
        if (n[0] == 'm') { v = -1; exit(v); }
        if (n[0] == 'e') { v = 256; exit(v); }
        v = 0x12345678; _exit(v);
        return 77;
    }
    TC_RUN(exit_minus_one); TC_RUN(exit_256); TC_RUN(underscore_exit_wide);
    return tc_end();
}
