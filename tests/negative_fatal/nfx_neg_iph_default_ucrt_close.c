/* TC
 * id:    nfx_neg_iph_default_ucrt_close
 * area:  negative_fatal
 * kind:  negative
 * funcs: _close
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _close ("If fd is a bad file descriptor, the invalid parameter handler is invoked")
 * note:  Probes exact fast-fail status 0xC0000409 for bad fds reaching the default handler. fd -2 is deliberately NOT here:
 *        the CRT special-cases -2 (_NO_CONSOLE_FILENO) to return EBADF without the handler; see nfx_neg_close_minus2_no_handler.
 */
#include "tc.h"
#include <io.h>

static volatile int v_fd;

TC_CASE(minus1) { DWORD c = 0; CHECK(tc_spawn_self("m1", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }
TC_CASE(beyond_table) { DWORD c = 0; CHECK(tc_spawn_self("big", &c, 20000)); CHECK_EQ_U(c, TC_STATUS_FASTFAIL); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        tc_iph_default();
        v_fd = n[0] == 'b' ? 100000 : -1;
        _close(v_fd);
        return 77;
    }
    TC_RUN(minus1); TC_RUN(beyond_table);
    return tc_end();
}
