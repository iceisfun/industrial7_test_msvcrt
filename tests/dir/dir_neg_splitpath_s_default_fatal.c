/* TC
 * id:    dir_neg_splitpath_s_default_fatal
 * area:  dir
 * kind:  negative
 * funcs:
 * procs: _splitpath_s
 * crt:   >=140
 * tier:  p0
 * conf:  probe
 * ref:   MS docs "Parameter validation": without a handler an invalid parameter terminates the process
 * note:  Case child_terminates is documented (process must not return normally: exit code not 0 and not 77).
 *        Case exact_status is the probe: expects the UCRT fast-fail status 0xC0000409. Fatal call runs in a child.
 */
#include "tc.h"

typedef int (__cdecl *sp_t)(const char *, char *, size_t, char *, size_t, char *, size_t, char *, size_t);

TC_CASE(child_terminates)
{
    DWORD code = 0; int ran = tc_spawn_self("nullpath", &code, 20000);
    CHECK(ran);
    CHECK(code != 0);
    CHECK(code != 77);
}
TC_CASE(exact_status)
{
    DWORD code = 0; int ran = tc_spawn_self("nullpath", &code, 20000);
    CHECK(ran);
    CHECK_EQ_U(code, TC_STATUS_FASTFAIL);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) {
        char d[_MAX_DRIVE], di[_MAX_DIR], f[_MAX_FNAME], e[_MAX_EXT];
        sp_t sp = (sp_t)tc_crt_proc("_splitpath_s");
        if (!sp) return 78;
        sp(NULL, d, _MAX_DRIVE, di, _MAX_DIR, f, _MAX_FNAME, e, _MAX_EXT);
        return 77;
    }
    TC_RUN(child_terminates); TC_RUN(exact_status);
    return tc_end();
}
