/* TC
 * id:    prc_neg_beginthread_null_start
 * area:  process
 * funcs: _beginthread _beginthreadex
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _beginthread/_beginthreadex (-1 / 0 on error, errno EINVAL for invalid argument)
 * note:  PROBE: a NULL start address is rejected: _beginthread returns -1, _beginthreadex returns 0, errno EINVAL,
 *        invalid-parameter handler invoked once. (msvcrt.dll has no validation, hence crt >=80.)
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

typedef unsigned (__stdcall *sx_t)(void *);
typedef void (__cdecl *s_t)(void *);
static volatile sx_t v_sx; static volatile s_t v_s;

TC_CASE(beginthread_null)
{
    uintptr_t r;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = _beginthread((s_t)v_s, 0, NULL);
    CHECK_EQ_I((intptr_t)r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(beginthreadex_null)
{
    uintptr_t r; unsigned id = 0x55;
    if (!tc_iph_install()) TC_SKIP("no iph api");
    tc_iph_count = 0; errno = 0;
    r = _beginthreadex(NULL, 0, (sx_t)v_sx, NULL, 0, &id);
    CHECK_EQ_I((intptr_t)r, 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(beginthread_null); TC_RUN(beginthreadex_null); return tc_end(); }
