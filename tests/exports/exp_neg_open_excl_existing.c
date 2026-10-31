/* TC
 * id:    exp_neg_open_excl_existing
 * area:  exports
 * kind:  negative
 * funcs: _open _close _unlink
 * procs: 
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _open: _O_EXCL with _O_CREAT returns -1 and errno EEXIST if the file exists
 * note:  File tc_exp_neg_open_excl.tmp is removed.
 */
#include "tc.h"
#define TC_STR2(x) #x
#define TC_STR(x) TC_STR2(x)
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>

static const char *volatile fn = "tc_exp_neg_open_excl_" TC_STR(TC_CRT_VER) ".tmp";

TC_CASE(create_then_excl_fails_eexist)
{
    int fd, fd2;
    _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_EXCL | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    errno = 0;
    fd2 = _open(fn, _O_CREAT | _O_EXCL | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd2, -1); CHECK_EQ_I(errno, EEXIST);
    if (fd2 >= 0) _close(fd2);
    if (fd >= 0) _close(fd);
}
TC_CASE(cleanup_ok) { CHECK_EQ_I(_unlink(fn), 0); }

int main(void) { tc_begin(); TC_RUN(create_then_excl_fails_eexist); TC_RUN(cleanup_ok); return tc_end(); }
