/* TC
 * id:    prc_neg_wspawnl_invalid_mode
 * area:  process
 * kind:  negative
 * funcs: _wspawnl
 * crt:   >=80
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _spawn, _wspawn functions; Parameter validation (EINVAL + invalid parameter handler)
 * note:  F: _wspawnl (no "p") does not search PATH and the test dir is not System32, so the bare name "cmd.exe" failed with
 *        ENOENT (errno 2) for ANY mode. The program is now the absolute path from GetSystemDirectoryW. Observed on Win10 22H2
 *        (19045): an invalid mode (99) with a valid program returns -1, errno EINVAL and one handler call. Stable.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(invalid_mode)
{
    intptr_t r; volatile int mode = 99; wchar_t cmd[MAX_PATH + 16]; UINT n = GetSystemDirectoryW(cmd, MAX_PATH);
    int i = 0; const wchar_t *tail = L"\\cmd.exe";
    CHECK(n > 0 && n < MAX_PATH); if (!(n > 0 && n < MAX_PATH)) return;
    while (tail[i]) { cmd[n + i] = tail[i]; i++; } cmd[n + i] = 0;   /* _wspawnl does not search PATH: a bare "cmd.exe" gives ENOENT for any mode */
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = _wspawnl(mode, cmd, L"cmd.exe", L"/c", L"exit 0", (const wchar_t *)NULL);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
}

int main(void) { tc_begin(); TC_RUN(invalid_mode); return tc_end(); }
