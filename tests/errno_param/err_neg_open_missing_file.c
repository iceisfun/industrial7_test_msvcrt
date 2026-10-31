/* TC
 * id:    err_neg_open_missing_file
 * area:  errno_param
 * funcs: _open __doserrno
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs _open (errno values)
 * note:  EEXIST for _O_CREAT|_O_EXCL on an existing file is documented.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

extern unsigned long *__cdecl __doserrno(void);
#define FN "tc_err_neg_open_missing_file.tmp"

TC_CASE(missing_rdonly)
{
    int fd; errno = 0; *__doserrno() = 0;
    fd = _open("tc_err_neg_open_missing_nonexistent.tmp", _O_RDONLY);
    CHECK_EQ_I(fd, -1); CHECK_EQ_I(errno, ENOENT); CHECK_EQ_U(*__doserrno(), 2);
    if (fd >= 0) _close(fd);
}
TC_CASE(excl_existing)
{
    int fd, fd2;
    DeleteFileA(FN);
    fd = _open(FN, _O_CREAT | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd >= 0) _close(fd);
    errno = 0;
    fd2 = _open(FN, _O_CREAT | _O_EXCL | _O_WRONLY, _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(fd2, -1); CHECK_EQ_I(errno, EEXIST);
    if (fd2 >= 0) _close(fd2);
    DeleteFileA(FN);
}
int main(void) { tc_begin(); TC_RUN(missing_rdonly); TC_RUN(excl_existing); return tc_end(); }
