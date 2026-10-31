/* TC
 * id:    prc_neg_spawn_empty_cmdname
 * area:  process
 * funcs: _spawnl _wspawnl _spawnv
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _spawn (errno ENOENT / EINVAL / ENOEXEC; return -1 on error)
 * note:  PROBES: an empty-string cmdname fails with -1 and errno in {ENOENT, EINVAL}. No assertion on the invalid-parameter
 *        handler (no handler is installed; if the CRT treated "" as invalid and the default handler is fatal this would crash,
 *        so a handler is installed where available but its count is not asserted).
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(spawnl_empty)
{
    volatile int m = _P_WAIT; const char *volatile p = ""; intptr_t r;
    tc_iph_install(); errno = 0;
    r = _spawnl(m, p, "x", (char *)NULL);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
}
TC_CASE(wspawnl_empty)
{
    volatile int m = _P_WAIT; const wchar_t *volatile p = L""; intptr_t r;
    tc_iph_install(); errno = 0;
    r = _wspawnl(m, p, L"x", (wchar_t *)NULL);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
}
TC_CASE(spawnv_empty)
{
    volatile int m = _P_WAIT; const char *volatile p = ""; intptr_t r; const char *av[2];
    av[0] = "x"; av[1] = NULL;
    tc_iph_install(); errno = 0;
    r = _spawnv(m, p, av);
    CHECK_EQ_I(r, -1); CHECK(errno == ENOENT || errno == EINVAL);
}

int main(void)
{
    tc_begin();
    TC_RUN(spawnl_empty); TC_RUN(wspawnl_empty); TC_RUN(spawnv_empty);
    return tc_end();
}
