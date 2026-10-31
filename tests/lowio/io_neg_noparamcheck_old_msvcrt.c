/* TC
 * id:    io_neg_noparamcheck_old_msvcrt
 * area:  lowio
 * funcs: _close _lseek _dup
 * crt:   msvcrt
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _close, _lseek, _dup (return -1 and EBADF for invalid descriptors)
 * note:  PROBE: msvcrt.dll (Win7 era) validates bad fds by returning -1/EBADF and does not terminate the process;
 *        no invalid-parameter handler is installed or asserted. Only fd -1 cases are used.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>

static volatile int bad = -1;

TC_CASE(close_bad)  { errno = 0; CHECK_EQ_I(_close(bad), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(lseek_bad)  { errno = 0; CHECK_EQ_I(_lseek(bad, 0, 0), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(dup_bad)    { errno = 0; CHECK_EQ_I(_dup(bad), -1); CHECK_EQ_I(errno, EBADF); }

int main(void) { tc_begin(); TC_RUN(close_bad); TC_RUN(lseek_bad); TC_RUN(dup_bad); return tc_end(); }
