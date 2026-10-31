/* TC
 * id:    err_errno_new_thread_zero
 * area:  errno_param
 * funcs: _errno
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   none (undocumented)
 * note:  PROBE: a newly created thread starts with errno==0 (per-thread data zero-initialised). Not explicitly documented.
 */
#include "tc.h"
#include <errno.h>

static volatile int seen = -1;
static DWORD WINAPI w(LPVOID a) { (void)a; seen = errno; return 0; }

TC_CASE(zero_in_new_thread)
{
    HANDLE h; errno = ERANGE;
    h = CreateThread(NULL, 0, w, NULL, 0, NULL); CHECK(h != NULL); if (!h) return;
    CHECK(WaitForSingleObject(h, 10000) == WAIT_OBJECT_0); CloseHandle(h);
    CHECK_EQ_I(seen, 0); errno = 0;
}
int main(void) { tc_begin(); TC_RUN(zero_in_new_thread); return tc_end(); }
