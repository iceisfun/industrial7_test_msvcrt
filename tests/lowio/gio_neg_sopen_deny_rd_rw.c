/* TC
 * id:    gio_neg_sopen_deny_rd_rw
 * area:  lowio
 * funcs: _sopen _close _unlink _read _write
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _sopen (share modes); Windows ERROR_SHARING_VIOLATION is mapped to errno EACCES
 * note:  Extends io_neg_sopen_share_deny: also checks the file is unchanged after the failed open. fd-slot leak check lives in gio_neg_probe_sopen_fd_leak.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_neg_sopen_deny_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(denyrw_blocks_read)
{
    int a, b; _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW, PM);
    CHECK(a >= 0);
    errno = 0; b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1); CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
TC_CASE(denyrw_blocks_write)
{
    int a, b; _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW, PM);
    CHECK(a >= 0);
    errno = 0; b = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1); CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
TC_CASE(denyrd_blocks_read)
{
    int a, b; _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDONLY | _O_BINARY, _SH_DENYRD, PM);
    CHECK(a >= 0);
    errno = 0; b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1); CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
TC_CASE(denywr_blocks_write)
{
    int a, b; _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDONLY | _O_BINARY, _SH_DENYWR, PM);
    CHECK(a >= 0);
    errno = 0; b = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1); CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
TC_CASE(file_intact_after_failure)
{
    int a, b; char buf[16] = {0}; _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW, PM);
    CHECK(a >= 0);
    if (a >= 0) _write(a, "hello", 5);
    errno = 0;
    b = _sopen(fn, _O_WRONLY | _O_TRUNC | _O_BINARY, _SH_DENYNO);   /* must fail before truncating */
    CHECK_EQ_I(b, -1);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    a = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(a >= 0);
    if (a >= 0) { CHECK_EQ_I(_read(a, buf, 15), 5); _close(a); }
    CHECK_STR(buf, "hello");
    _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(denyrw_blocks_read); TC_RUN(denyrw_blocks_write); TC_RUN(denyrd_blocks_read);
    TC_RUN(denywr_blocks_write); TC_RUN(file_intact_after_failure);
    return tc_end();
}
