/* TC
 * id:    io_neg_sopen_share_deny
 * area:  lowio
 * kind:  negative
 * funcs: _sopen _close _unlink
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _sopen (_SH_DENYRW, _SH_DENYWR, _SH_DENYRD); Windows ERROR_SHARING_VIOLATION maps to EACCES
 * note:  A sharing violation is reported by the CRT as errno EACCES.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER) /* per-CRT file names: targets may run concurrently in one directory */
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_neg_sopen_share_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(denyrw_blocks_read)
{
    int a, b;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYRW, PM);
    CHECK(a >= 0);
    errno = 0;
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1);
    CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    errno = 0;
    b = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1);
    CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) CHECK_EQ_I(_close(a), 0);
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(b >= 0);
    if (b >= 0) _close(b);
    _unlink(fn);
}
TC_CASE(denywr_blocks_write_allows_read)
{
    int a, b;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDONLY | _O_BINARY, _SH_DENYWR, PM);
    CHECK(a >= 0);
    errno = 0;
    b = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1);
    CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    errno = 0;
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(b >= 0);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
TC_CASE(denyrd_blocks_read)
{
    int a, b;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYRD, PM);
    CHECK(a >= 0);
    errno = 0;
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(b, -1);
    CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
TC_CASE(second_denyrw_conflicts_with_open)
{
    int a, b;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, PM);
    CHECK(a >= 0);
    errno = 0;
    b = _sopen(fn, _O_RDWR | _O_BINARY, _SH_DENYRW);   /* new opener denies others, but a already has access */
    CHECK_EQ_I(b, -1);
    CHECK_EQ_I(errno, EACCES);
    if (b >= 0) _close(b);
    if (a >= 0) _close(a);
    _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(denyrw_blocks_read); TC_RUN(denywr_blocks_write_allows_read);
    TC_RUN(denyrd_blocks_read); TC_RUN(second_denyrw_conflicts_with_open);
    return tc_end();
}
