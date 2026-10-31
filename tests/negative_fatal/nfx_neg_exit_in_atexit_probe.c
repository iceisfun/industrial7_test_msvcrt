/* TC
 * id:    nfx_neg_exit_in_atexit_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: exit  _exit
 * procs: _onexit
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.22.4.4
 * note:  exit() inside an exit handler is undefined per C11 7.22.4.4p2 (exit called more than once). Case 1 only asserts the process terminates with status 7 within the timeout. Case 2 (an earlier-registered handler calls _exit(8)) records whether remaining handlers still run; asserts only that the status is 7 or 8 and prints which.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

TC_DECL_PROC(_onexit_t, _onexit, (_onexit_t));


static int __cdecl h_exit7(void) { exit(7); return 0; }
static int __cdecl h_u8(void) { _exit(8); return 0; }

TC_CASE(nested_exit_terminates)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("one", &code, 20000);
    CHECK(ran); CHECK_EQ_U(code, 7);
}
TC_CASE(nested_exit_other_handlers_recorded)
{
    DWORD code = 0xdead; int ran = tc_spawn_self("two", &code, 20000);
    CHECK(ran);
    CHECK(code == 7 || code == 8);
    tc_write(code == 8 ? "TC INFO remaining handlers still run after nested exit (8)\n"
                       : "TC INFO nested exit terminates immediately (7)\n");
}
int main(void)
{
    const char *c;
    tc_begin();
    TC_LOAD_PROC(_onexit);
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 't') { tc_p__onexit(h_u8); }
        tc_p__onexit(h_exit7);
        exit(0);
    }
    TC_RUN(nested_exit_terminates); TC_RUN(nested_exit_other_handlers_recorded);
    return tc_end();
}
