/* TC
 * id:    gio_sopen_share_matrix
 * area:  lowio
 * funcs: _sopen _close _unlink _write
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _sopen (shflag table: _SH_DENYRW, _SH_DENYWR, _SH_DENYRD, _SH_DENYNO; optional pmode argument)
 * note:  Allowed sharing combinations (map to CreateFile share flags). Conflicting combinations are in gio_neg_sopen_deny_rd_rw.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <io.h>
#include <fcntl.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_sopen_share_matrix_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

static void mkfile(void)
{
    int fd = _sopen(fn, _O_CREAT | _O_TRUNC | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    if (fd >= 0) { _write(fd, "abc", 3); _close(fd); }
}
static void cl(int a, int b) { if (a >= 0) _close(a); if (b >= 0) _close(b); }

TC_CASE(denyno_denyno_rw)
{
    int a, b;
    _unlink(fn); mkfile();
    a = _sopen(fn, _O_RDWR | _O_BINARY, _SH_DENYNO);
    b = _sopen(fn, _O_RDWR | _O_BINARY, _SH_DENYNO);
    CHECK(a >= 3); CHECK(b >= 3); CHECK(a != b);
    cl(a, b); _unlink(fn);
}
TC_CASE(denywr_allows_read)
{
    int a, b;
    _unlink(fn); mkfile();
    a = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYWR);
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(a >= 0); CHECK(b >= 0);
    cl(a, b); _unlink(fn);
}
TC_CASE(denyrd_allows_write)
{
    int a, b;
    _unlink(fn); mkfile();
    a = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYRD);
    b = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYNO);
    CHECK(a >= 0); CHECK(b >= 0);
    cl(a, b); _unlink(fn);
}
TC_CASE(denywr_then_read_denywr)
{
    int a, b;
    _unlink(fn); mkfile();
    a = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYWR);
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYWR);
    CHECK(a >= 0); CHECK(b >= 0);
    cl(a, b); _unlink(fn);
}
TC_CASE(vararg_pmode)
{
    int a, b;
    _unlink(fn);
    a = _sopen(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYNO, _S_IWRITE);
    CHECK(a >= 3);
    b = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(b >= 3); CHECK(a != b);
    if (a >= 0) CHECK_EQ_I(_write(a, "z", 1), 1);
    cl(a, b); _unlink(fn);
}
TC_CASE(fd_close_releases_share)
{
    int a, b;
    _unlink(fn); mkfile();
    a = _sopen(fn, _O_RDWR | _O_BINARY, _SH_DENYNO);
    CHECK(a >= 0);
    if (a >= 0) CHECK_EQ_I(_close(a), 0);
    b = _sopen(fn, _O_WRONLY | _O_BINARY, _SH_DENYRW);
    CHECK(b >= 0);
    cl(-1, b); _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(denyno_denyno_rw); TC_RUN(denywr_allows_read); TC_RUN(denyrd_allows_write);
    TC_RUN(denywr_then_read_denywr); TC_RUN(vararg_pmode); TC_RUN(fd_close_releases_share);
    return tc_end();
}
