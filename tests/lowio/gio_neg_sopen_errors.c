/* TC
 * id:    gio_neg_sopen_errors
 * area:  lowio
 * funcs: _sopen _close _unlink _read
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _open/_sopen return value and errno table (ENOENT, EEXIST)
 * note:  Null path / bad shflag are covered in io_neg_sopen_*.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_gio_neg_sopen_errors_" TCV ".tmp";
static const char *volatile badp = "tc_gio_nonexistent_dir_" TCV "/x";
#define PM (_S_IREAD | _S_IWRITE)

TC_CASE(missing_no_creat)
{
    int fd; _unlink(fn);
    errno = 0; fd = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT);
    if (fd >= 0) _close(fd);
}
TC_CASE(creat_excl_exists)
{
    int fd; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    CHECK(fd >= 0); if (fd >= 0) _close(fd);
    errno = 0; fd = _sopen(fn, _O_CREAT | _O_EXCL | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, EEXIST);
    if (fd >= 0) _close(fd);
    _unlink(fn);
}
TC_CASE(missing_dir_component)
{
    int fd;
    errno = 0; fd = _sopen(badp, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT);
    if (fd >= 0) _close(fd);
}
TC_CASE(creat_excl_does_not_modify_existing)
{
    int fd; char b[16] = {0}; _unlink(fn);
    fd = _sopen(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    if (fd >= 0) { _write(fd, "keep", 4); _close(fd); }
    fd = _sopen(fn, _O_CREAT | _O_EXCL | _O_TRUNC | _O_WRONLY | _O_BINARY, _SH_DENYNO, PM);
    CHECK_EQ_I(fd, -1);
    if (fd >= 0) _close(fd);
    fd = _sopen(fn, _O_RDONLY | _O_BINARY, _SH_DENYNO);
    CHECK(fd >= 0);
    if (fd >= 0) { CHECK_EQ_I(_read(fd, b, 15), 4); _close(fd); }
    CHECK_STR(b, "keep");
    _unlink(fn);
}
int main(void)
{
    tc_begin();
    TC_RUN(missing_no_creat); TC_RUN(creat_excl_exists); TC_RUN(missing_dir_component);
    TC_RUN(creat_excl_does_not_modify_existing);
    return tc_end();
}
