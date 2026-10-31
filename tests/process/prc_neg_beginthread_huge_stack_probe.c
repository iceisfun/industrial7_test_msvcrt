/* TC
 * id:    prc_neg_beginthread_huge_stack_probe
 * area:  process
 * funcs: _beginthread
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _beginthread: returns -1 on error, errno EAGAIN / EINVAL / EACCES
 * note:  PROBE: machine dependent (commit limit). The stack_size parameter is 32-bit unsigned, so 0xF0000000 (3.75 GB committed)
 *        is the largest practical request. A host with ample free commit may legitimately create the thread, so the case accepts
 *        either outcome: -1 => errno in {EAGAIN,EACCES,EINVAL,ENOMEM} and thread never ran; success => thread runs.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

static volatile LONG g_ran;
static void __cdecl worker(void *p) { (void)p; InterlockedExchange(&g_ran, 1); }
static uintptr_t g_r; static int g_errno;

TC_CASE(consistent_outcome)
{
    if (g_r == (uintptr_t)-1) {   /* failure path: errno mapped, no thread */
        CHECK(g_errno == EAGAIN || g_errno == EACCES || g_errno == EINVAL || g_errno == ENOMEM);
        Sleep(200); CHECK_EQ_I(g_ran, 0);
    } else {                      /* host has the commit: thread must have been created and run */
        CHECK(g_r != 0);
        { int i; for (i = 0; i < 100 && !g_ran; i++) Sleep(50); }
        CHECK_EQ_I(g_ran, 1);
    }
}
int main(void)
{
    tc_begin();
    errno = 0;
    g_r = _beginthread(worker, 0xF0000000u, NULL);
    g_errno = errno;
    TC_RUN(consistent_outcome);
    return tc_end();
}
