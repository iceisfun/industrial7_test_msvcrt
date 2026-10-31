/* TC
 * id:    prc_neg_spawnl_directory_probe
 * area:  process
 * kind:  negative
 * funcs: _spawnl
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _spawn functions (return -1 on error, errno EACCES/ENOENT/...)
 * note:  Probes the errno that results from asking CreateProcess to run a directory (%SystemRoot%):
 *        Most likely EACCES (CreateProcess ERROR_ACCESS_DENIED); ENOENT/ENOEXEC also accepted because the CRT may
 *        append extensions (.exe/.cmd/...) to an extension-less path before trying. Only the -1 return is certain.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static char dirpath[MAX_PATH];

TC_CASE(spawn_directory)
{
    intptr_t r; int e;
    errno = 0;
    r = _spawnl(_P_WAIT, dirpath, dirpath, (char *)NULL);
    e = errno;
    CHECK_EQ_I(r, -1);
    CHECK(e != 0);
    { int ok = (e == EACCES || e == ENOENT || e == ENOEXEC); CHECK(ok); }   /* probe */
}

int main(void)
{
    tc_begin();
    GetWindowsDirectoryA(dirpath, MAX_PATH);
    TC_RUN(spawn_directory);
    return tc_end();
}
