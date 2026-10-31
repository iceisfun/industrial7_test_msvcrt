/* TC
 * id:    io_access_basic
 * area:  lowio
 * funcs: _access _open _close _chmod _unlink
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _access (modes 00 existence, 02 write, 04 read, 06 read/write; read-only file fails write check with EACCES)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

static const char *volatile fn = "tc_io_access_basic.tmp";
static void make(int ro)
{
    int fd;
    _chmod(fn, _S_IREAD | _S_IWRITE); _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _close(fd);
    if (ro) _chmod(fn, _S_IREAD);
}
static void done(void) { _chmod(fn, _S_IREAD | _S_IWRITE); _unlink(fn); }
TC_CASE(existing_mode0)  { make(0); CHECK_EQ_I(_access(fn, 0), 0); done(); }
TC_CASE(rw_file_modes)   { make(0); CHECK_EQ_I(_access(fn, 2), 0); CHECK_EQ_I(_access(fn, 4), 0); CHECK_EQ_I(_access(fn, 6), 0); done(); }
TC_CASE(ro_file_write_denied)
{
    make(1);
    errno = 0;
    CHECK_EQ_I(_access(fn, 2), -1);
    CHECK_EQ_I(errno, EACCES);
    done();
}
TC_CASE(ro_file_read_and_rw)
{
    make(1);
    CHECK_EQ_I(_access(fn, 0), 0);
    CHECK_EQ_I(_access(fn, 4), 0);
    errno = 0;
    CHECK_EQ_I(_access(fn, 6), -1);
    CHECK_EQ_I(errno, EACCES);
    done();
}
TC_CASE(directory_exists) { CHECK_EQ_I(_access(".", 0), 0); }
int main(void)
{
    tc_begin();
    TC_RUN(existing_mode0); TC_RUN(rw_file_modes); TC_RUN(ro_file_write_denied);
    TC_RUN(ro_file_read_and_rw); TC_RUN(directory_exists);
    return tc_end();
}
