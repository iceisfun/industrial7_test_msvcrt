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
 * note:  Machine dependent (commit limit): a host with >4GB free commit may legitimately create the thread. Probes failure mapping (CreateThread fails with insufficient memory/resources) for an impossible 4GB committed stack.
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
    if (r) { WaitForSingleObject((HANDLE)r, 10000); CloseHandle((HANDLE)r); tc_write("TC NOTE huge stack thread unexpectedly succeeded\n"); }
    CHECK_EQ_U(r, 0);
    CHECK(errno == EAGAIN || errno == EACCES || errno == EINVAL || errno == ENOMEM);
}
int main(void) { tc_begin(); TC_RUN(huge_stack); return tc_end(); }
