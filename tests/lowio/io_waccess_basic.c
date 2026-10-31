/* TC
 * id:    io_waccess_basic
 * area:  lowio
 * funcs: _waccess _wopen _close _wunlink
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _waccess, _wopen, _wunlink
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

/* name contains U+00E9 and U+4E2D (BMP, non-ASCII) */
static const wchar_t *volatile wfn = L"tc_io_waccess_\x00e9\x4e2d.tmp";
static const wchar_t *volatile wmissing = L"tc_io_waccess_missing_\x4e2d.tmp";
static void mkw(void)
{
    int fd;
    _wunlink(wfn);
    fd = _wopen(wfn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
}
TC_CASE(existing)  { mkw(); CHECK_EQ_I(_waccess(wfn, 0), 0); CHECK_EQ_I(_waccess(wfn, 6), 0); _wunlink(wfn); }
TC_CASE(missing)   { errno = 0; CHECK_EQ_I(_waccess(wmissing, 0), -1); CHECK_EQ_I(errno, ENOENT); }
TC_CASE(gone_after_unlink)
{
    mkw();
    CHECK_EQ_I(_wunlink(wfn), 0);
    errno = 0;
    CHECK_EQ_I(_waccess(wfn, 0), -1);
    CHECK_EQ_I(errno, ENOENT);
}
TC_CASE(wide_dot) { CHECK_EQ_I(_waccess(L".", 0), 0); }
int main(void) { tc_begin(); TC_RUN(existing); TC_RUN(missing); TC_RUN(gone_after_unlink); TC_RUN(wide_dot); return tc_end(); }
