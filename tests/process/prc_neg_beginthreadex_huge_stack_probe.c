/* TC
 * id:    prc_neg_beginthreadex_huge_stack_probe
 * area:  process
 * funcs: _beginthreadex
 * procs: 
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _beginthreadex errno values EAGAIN, EACCES, EINVAL
 * note:  G (missing datum): on the 4 GB reference VM (Win10 22H2 19045) the 0xFFFFFFF0-byte stack request is NOT stable. Run
 *        raw: _beginthreadex returned a handle (0xb0) and the original r == 0 check failed; a later in-VM probe of the same
 *        call recorded r=0 errno=22 (EINVAL), and the corpus runs of the amended test SKIP (thread created). So success vs
 *        failure depends on the commit limit / VM state and is nondeterministic here. The case SKIPs when the thread was
 *        created and asserts the failure mapping (EAGAIN/EACCES/EINVAL/ENOMEM) only when creation fails; it asserts nothing on
 *        the reference until a host with a known, fixed commit limit is available. Demoted to probe.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static unsigned __stdcall worker(void *p) { (void)p; return 0; }

TC_CASE(huge_stack)
{
    uintptr_t r; unsigned id = 0;
    errno = 0;
    r = _beginthreadex(NULL, 0xFFFFFFF0u, worker, NULL, 0, &id);
    if (r) {   /* host can commit 4 GB (observed on the 4 GB reference VM, handle returned): the failure path is not reachable here */
        WaitForSingleObject((HANDLE)r, 10000); CloseHandle((HANDLE)r);
        TC_SKIP("host can commit the 4 GB stack; failure mapping not observable");
    }
    CHECK_EQ_U(r, 0);
    CHECK(errno == EAGAIN || errno == EACCES || errno == EINVAL || errno == ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(huge_stack); return tc_end(); }
