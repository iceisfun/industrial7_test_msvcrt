/* TC
 * id:    prc_neg_spawn_nonexistent
 * area:  process
 * funcs: _spawnl _spawnv _wspawnl _spawnlp
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _spawn, _wspawn functions: errno ENOENT = file or path not found; return -1
 * note:  No invalid-parameter handler call is asserted.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(spawnl_missing)
{
    volatile int m = _P_WAIT; intptr_t r; errno = 0;
    r = _spawnl(m, "C:\\tc_no_such_dir\\x.exe", "x.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(spawnv_missing)
{
    volatile int m = _P_WAIT; intptr_t r; const char *av[2]; av[0] = "x.exe"; av[1] = NULL; errno = 0;
    r = _spawnv(m, "C:\\tc_no_such_dir\\x.exe", av);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wspawnl_missing)
{
    volatile int m = _P_WAIT; intptr_t r; errno = 0;
    r = _wspawnl(m, L"C:\\tc_no_such_dir\\x.exe", L"x.exe", (wchar_t *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(spawnlp_bare_missing)
{
    volatile int m = _P_WAIT; intptr_t r; errno = 0;
    r = _spawnlp(m, "tc_no_such_program_zq91.exe", "tc_no_such_program_zq91.exe", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ENOENT);
}

int main(void)
{
    tc_begin();
    TC_RUN(spawnl_missing); TC_RUN(spawnv_missing); TC_RUN(wspawnl_missing); TC_RUN(spawnlp_bare_missing);
    return tc_end();
}
