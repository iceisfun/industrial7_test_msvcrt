/* TC
 * id:    prc_neg_spawn_wait_nonexistent_detach
 * area:  process
 * funcs: _spawnl _wspawnl
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions: returns -1 on error, errno ENOENT = file or path not found
 * note:  _P_DETACH and _P_NOWAITO of a missing file; the return is -1, not a process handle.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(detach_missing)
{
    volatile int m = _P_DETACH; intptr_t r; errno = 0;
    r = _spawnl(m, "C:\\tc_no_such_dir\\x.exe", "x.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(nowaito_missing)
{
    volatile int m = _P_NOWAITO; intptr_t r; errno = 0;
    r = _spawnl(m, "C:\\tc_no_such_dir\\x.exe", "x.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wnowaito_missing)
{
    volatile int m = _P_NOWAITO; intptr_t r; errno = 0;
    r = _wspawnl(m, L"C:\\tc_no_such_dir\\x.exe", L"x.exe", (wchar_t *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}

int main(void)
{
    tc_begin();
    TC_RUN(detach_missing); TC_RUN(nowaito_missing); TC_RUN(wnowaito_missing);
    return tc_end();
}
