/* TC
 * id:    io_fstat64_regular
 * area:  lowio
 * funcs: _fstat64 _open _write _close
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fstat, _fstat64 (st_size, st_mode, st_nlink, st_mtime)
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define FN "tc_io_fstat64_regular.tmp"

TC_CASE(regular)
{
    static const char data[] = "0123456789abcdef";
    struct _stat64 st; int fd, r;
    rm(FN);
    fd = _open(FN, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, data, 16), 16);
    memset(&st, 0x5a, sizeof st);
    r = _fstat64(fd, &st);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_I(st.st_size, 16);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFREG);
    CHECK_EQ_I(st.st_mode & (_S_IREAD | _S_IWRITE), _S_IREAD | _S_IWRITE);
    CHECK_EQ_I(st.st_nlink, 1);
    CHECK(st.st_mtime != 0);
    _close(fd);
    rm(FN);
}
int main(void) { tc_begin(); TC_RUN(regular); return tc_end(); }
