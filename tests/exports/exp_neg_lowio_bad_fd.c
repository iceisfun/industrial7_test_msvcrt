/* TC
 * id:    exp_neg_lowio_bad_fd
 * area:  exports
 * kind:  negative
 * funcs: _read _write _lseek _dup _commit
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _read/_write/_lseek/_dup/_commit: -1 with errno EBADF
 * note:  fd -1 is invalid. UCRT/msvcr80+ also call the invalid-parameter handler (installed here, count not asserted); msvcrt has none.
 */
#include "tc.h"
#include <io.h>
#include <errno.h>
#include <stdio.h>

static volatile int bad = -1;
static char buf[8];

TC_CASE(read_minus1_ebadf) { errno = 0; CHECK_EQ_I(_read(bad, buf, 4), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(write_minus1_ebadf) { errno = 0; CHECK_EQ_I(_write(bad, "abc", 3), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(lseek_minus1_ebadf) { errno = 0; CHECK_EQ_I(_lseek(bad, 0, 0), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(dup_minus1_ebadf) { errno = 0; CHECK_EQ_I(_dup(bad), -1); CHECK_EQ_I(errno, EBADF); }
TC_CASE(commit_minus1_ebadf) { errno = 0; CHECK_EQ_I(_commit(bad), -1); CHECK_EQ_I(errno, EBADF); }

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(read_minus1_ebadf); TC_RUN(write_minus1_ebadf); TC_RUN(lseek_minus1_ebadf);
    TC_RUN(dup_minus1_ebadf); TC_RUN(commit_minus1_ebadf);
    return tc_end();
}
