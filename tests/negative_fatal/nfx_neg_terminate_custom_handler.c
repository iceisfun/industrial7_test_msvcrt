/* TC
 * id:    nfx_neg_terminate_custom_handler
 * area:  negative_fatal
 * kind:  negative
 * funcs: terminate set_terminate
 * procs: 
 * crt:   ucrtbase
 * tier:  p1
 * conf:  documented
 * ref:   MS docs set_terminate, terminate
 * note:  Previous handler returned by the first set_terminate is not asserted.
 */
#include "tc.h"
typedef void (__cdecl *th_t)(void);
void __cdecl terminate(void);
th_t __cdecl set_terminate(th_t);
static void __cdecl h(void) { _exit(81); }
TC_CASE(handler_called) { DWORD c = 0; CHECK(tc_spawn_self("t", &c, 20000)); CHECK_EQ_U(c, 81); }
TC_CASE(returns_previous) { DWORD c = 0; CHECK(tc_spawn_self("prev", &c, 20000)); CHECK_EQ_U(c, 0); }
int main(void)
{
    const char *n; tc_begin();
    if ((n = tc_child_name()) != NULL) {
        if (n[0] == 't') { set_terminate(h); terminate(); return 77; }
        set_terminate(h);
        if (set_terminate(NULL) != h) return 1;
        return 0;
    }
    TC_RUN(handler_called); TC_RUN(returns_previous); return tc_end();
}
