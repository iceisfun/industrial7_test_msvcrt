/* TC
 * id:    nfx_neg_iph_handler_exit_custom
 * area:  negative_fatal
 * kind:  negative
 * funcs: _set_invalid_parameter_handler _close
 * procs: 
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _set_invalid_parameter_handler, _close (returns -1, errno EBADF)
 * note:  A custom handler may terminate the process with its own code, or return so the function reports the error.
 */
#include "tc.h"
#include <stdlib.h>
#include <io.h>
#include <errno.h>
static volatile int v_fd = -1;
static void __cdecl h_exit(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; _exit(33); }
static void __cdecl h_xp(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; ExitProcess(34); }
static void __cdecl h_ret(const wchar_t *a, const wchar_t *b, const wchar_t *c, unsigned int d, uintptr_t e) { (void)a;(void)b;(void)c;(void)d;(void)e; }

TC_CASE(handler_exit) { DWORD c = 0; CHECK(tc_spawn_self("exit", &c, 20000)); CHECK_EQ_U(c, 33); }
TC_CASE(handler_exitprocess) { DWORD c = 0; CHECK(tc_spawn_self("xp", &c, 20000)); CHECK_EQ_U(c, 34); }
TC_CASE(handler_returns) { DWORD c = 0; CHECK(tc_spawn_self("ret", &c, 20000)); CHECK_EQ_U(c, 35); }

int main(void)
{
    const char *n;
    tc_begin();
    if ((n = tc_child_name()) != NULL) {
        if (n[0] == 'e') { _set_invalid_parameter_handler(h_exit); _close(v_fd); return 77; }
        if (n[0] == 'x') { _set_invalid_parameter_handler(h_xp); _close(v_fd); return 77; }
        _set_invalid_parameter_handler(h_ret);
        errno = 0;
        { int r = _close(v_fd); return (r == -1 && errno == EBADF) ? 35 : 36; }
    }
    TC_RUN(handler_exit); TC_RUN(handler_exitprocess); TC_RUN(handler_returns);
    return tc_end();
}
