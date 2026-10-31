/* TC
 * id:    dir_neg_probe_findclose_errno
 * area:  dir
 * kind:  negative
 * funcs: _findclose
 * procs: _findfirst64i32
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _findclose
 * note:  Probes errno after _findclose(-1): expected EINVAL (FindClose fails with ERROR_INVALID_HANDLE, which the
 *        CRT's OS-error mapping turns into EBADF in some releases). Most likely EINVAL for invalid parameter; EBADF accepted.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

TC_CASE(errno_value)
{
    errno = 0;
    CHECK_EQ_I(_findclose((intptr_t)-1), -1);
    CHECK(errno == EINVAL || errno == EBADF);
}
int main(void) { tc_begin(); TC_RUN(errno_value); return tc_end(); }
