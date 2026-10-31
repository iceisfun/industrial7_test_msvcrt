/* TC
 * id:    nfx_neg_terminate_default_aborts
 * area:  negative_fatal
 * kind:  negative
 * funcs: terminate
 * procs: 
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs terminate (default handler calls abort); abort exit code 3
 * note:  ucrtbase exports C-linkage terminate.
 */
#include "tc.h"
typedef void (__cdecl *th_t)(void);
void __cdecl terminate(void);
th_t __cdecl set_terminate(th_t);
TC_CASE(default_aborts) { DWORD c = 0; CHECK(tc_spawn_self("t", &c, 20000)); CHECK_EQ_U(c, 3); }
int main(void) { tc_begin(); if (tc_child_name()) { terminate(); return 77; } TC_RUN(default_aborts); return tc_end(); }
