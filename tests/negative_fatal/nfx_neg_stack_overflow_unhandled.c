/* TC
 * id:    nfx_neg_stack_overflow_unhandled
 * area:  negative_fatal
 * kind:  negative
 * funcs: memset
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   Windows: unhandled STATUS_STACK_OVERFLOW terminates the process with that status
 * note:  Unbounded non-tail recursion in a child; no handler.
 */
#include "tc.h"
#include <string.h>

static int __attribute__((noinline)) rec(volatile int n)
{
    volatile char buf[1024];
    memset((void *)buf, n, sizeof buf);
    return rec(n + 1) + buf[0] + buf[1023];
}
TC_CASE(overflow_status) { DWORD c = 0; CHECK(tc_spawn_self("so", &c, 20000)); CHECK_EQ_U(c, 0xC00000FD); }
int main(void) { tc_begin(); if (tc_child_name()) { volatile int r = rec(0); return 77 + r; } TC_RUN(overflow_status); return tc_end(); }
