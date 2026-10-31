/* TC
 * id:    io_umask_plain_all
 * area:  lowio
 * funcs: _umask
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _umask
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(roundtrip_values)
{
    int old = _umask(0);
    volatile int m0 = 0, m1 = _S_IWRITE, m2 = _S_IREAD | _S_IWRITE, m3 = _S_IREAD;
    CHECK_EQ_I(_umask(m1), 0);
    CHECK_EQ_I(_umask(m2), _S_IWRITE);
    CHECK_EQ_I(_umask(m0), _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(_umask(m3), 0);
    CHECK_EQ_I(_umask(m3), _S_IREAD);
    _umask(old);
}
int main(void) { tc_begin(); TC_RUN(roundtrip_values); return tc_end(); }
