/* TC
 * id:    nfx_neg_terminate_handler_returns_probe
 * area:  negative_fatal
 * kind:  negative
 * funcs: terminate set_terminate
 * procs: 
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   C++ [except.terminate]; MS docs terminate
 * note:  PROBES that a returning terminate handler leads to abort (exit code 3).
 */
#include "tc.h"
typedef void (__cdecl *th_t)(void);
void __cdecl terminate(void);
th_t __cdecl set_terminate(th_t);
static void __cdecl h(void) { }
TC_CASE(returns_then_abort) { DWORD c = 0; CHECK(tc_spawn_self("t", &c, 20000)); CHECK_EQ_U(c, 3); }
int main(void) { tc_begin(); if (tc_child_name()) { set_terminate(h); terminate(); return 77; } TC_RUN(returns_then_abort); return tc_end(); }
