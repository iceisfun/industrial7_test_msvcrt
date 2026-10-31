/* TC
 * id:    io_umask_basic
 * area:  lowio
 * funcs: _umask _umask_s
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _umask, _umask_s
 * note:  _umask_s exists in msvcr80+; crt >=90 because of import libs
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

TC_CASE(umask_returns_previous)
{
    int old = _umask(0), r;
    r = _umask(_S_IWRITE);
    CHECK_EQ_I(r, 0);
    r = _umask(_S_IREAD | _S_IWRITE);
    CHECK_EQ_I(r, _S_IWRITE);
    r = _umask(0);
    CHECK_EQ_I(r, _S_IREAD | _S_IWRITE);
    _umask(old);
}
TC_CASE(umask_s_returns_old)
{
    int old = _umask(0), prev = -1;
    CHECK_EQ_I(_umask_s(_S_IWRITE, &prev), 0);
    CHECK_EQ_I(prev, 0);
    prev = -1;
    CHECK_EQ_I(_umask_s(_S_IREAD, &prev), 0);
    CHECK_EQ_I(prev, _S_IWRITE);
    CHECK_EQ_I(_umask(0), _S_IREAD);
    _umask(old);
}
TC_CASE(restore)
{
    int orig = _umask(_S_IREAD), now;
    _umask(orig);
    now = _umask(orig);
    CHECK_EQ_I(now, orig);
}
int main(void) { tc_begin(); TC_RUN(umask_returns_previous); TC_RUN(umask_s_returns_old); TC_RUN(restore); return tc_end(); }
