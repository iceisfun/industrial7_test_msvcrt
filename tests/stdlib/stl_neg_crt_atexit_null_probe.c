/* TC
 * id:    stl_neg_crt_atexit_null_probe
 * area:  stdlib
 * funcs: _crt_atexit
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   UCRT internal export _crt_atexit
 * note:  Probes what _crt_atexit(NULL) does: expected to return 0 (registration is not validated) and a later
 *        exit-by-return to complete normally (NULL entry skipped, exit code 0). Both are child processes.
 */
#include "tc.h"
extern int __cdecl _crt_atexit(void (__cdecl *)(void));

TC_CASE(null_return_value)
{
    DWORD code = 0; int ran = tc_spawn_self("ret", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 100);  /* 100 = registration returned 0 */
}
TC_CASE(null_exit_normal)
{
    DWORD code = 99; int ran = tc_spawn_self("exit", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, 0);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        if (c[0] == 'r') { int r = _crt_atexit(NULL); ExitProcess(r == 0 ? 100 : 101); }
        _crt_atexit(NULL);
        return 0;
    }
    TC_RUN(null_return_value); TC_RUN(null_exit_normal);
    return tc_end();
}
