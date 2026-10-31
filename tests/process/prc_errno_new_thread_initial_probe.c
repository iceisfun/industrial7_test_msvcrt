/* TC
 * id:    prc_errno_new_thread_initial_probe
 * area:  process
 * funcs: _beginthreadex _errno
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _beginthreadex (per-thread CRT data)
 * note:  Probes that a new thread's per-thread data is zero-initialised so errno starts at 0 regardless of the parent's value.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static volatile int seen = -1;
static unsigned __stdcall worker(void *p) { (void)p; seen = errno; return 0; }

TC_CASE(new_thread_errno_zero)
{
    HANDLE h; unsigned id = 0;
    errno = ERANGE;
    h = (HANDLE)_beginthreadex(NULL, 0, worker, NULL, 0, &id);
    CHECK(h != NULL && h != (HANDLE)-1);
    if (!h) return;
    CHECK_EQ_I(WaitForSingleObject(h, 10000), WAIT_OBJECT_0);
    CloseHandle(h);
    CHECK_EQ_I(seen, 0);
    CHECK_EQ_I(errno, ERANGE);
    errno = 0;
}
int main(void) { tc_begin(); TC_RUN(new_thread_errno_zero); return tc_end(); }
