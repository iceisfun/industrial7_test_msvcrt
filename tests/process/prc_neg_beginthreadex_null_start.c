/* TC
 * id:    prc_neg_beginthreadex_null_start
 * area:  process
 * funcs: _beginthreadex
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _beginthreadex: returns 0 on error; errno EINVAL for invalid argument
 * note:  PROBE: NULL start_address is rejected via the invalid-parameter path (r==0, errno EINVAL, handler called once).
 *        The docs do not spell out NULL start_address explicitly. msvcrt.dll does not validate, hence crt >=80.
 */
#include "tc.h"
#include <process.h>
#include <errno.h>

TC_CASE(null_start)
{
    uintptr_t r; unsigned id = 0xabcd;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _beginthreadex(NULL, 0, NULL, NULL, 0, &id);
    CHECK_EQ_U(r, 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    if (r) { WaitForSingleObject((HANDLE)r, 1000); CloseHandle((HANDLE)r); }
}
int main(void) { tc_begin(); TC_RUN(null_start); return tc_end(); }
