/* TC
 * id:    ucr_neg_purecall_default_child
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _purecall
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _purecall; runtime error R6025 pure virtual function call
 * note:  Documented: default behaviour terminates the program abnormally. Exact status is in ucr_neg_purecall_exact_code_probe.
 */
#include "tc.h"

TC_CASE(terminates)
{
    DWORD code = 0;
    CHECK(tc_spawn_self("pc", &code, 20000));
    CHECK(code != 77);
    CHECK(code != 0);
    CHECK(code != 90);   /* 90 = export missing in child */
    CHECK(code != 0xdead0001u);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        void (__cdecl *pc)(void) = (void (__cdecl *)(void))tc_crt_proc("_purecall");
        if (!pc) ExitProcess(90);
        pc();
        return 77;
    }
    TC_RUN(terminates);
    return tc_end();
}
