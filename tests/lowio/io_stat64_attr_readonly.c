/* TC
 * id:    io_stat64_attr_readonly
 * area:  lowio
 * funcs: _stat64 _chmod _open _close _unlink
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _stat (st_mode: _S_IWRITE set unless file is read-only), _chmod
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

#define FN "tc_io_stat64_attr_readonly.tmp"

TC_CASE(readonly_toggle)
{
    struct _stat64 st; int fd;
    rm(FN);
    fd = _open(FN, _O_CREAT | _O_WRONLY | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    _close(fd);
    CHECK_EQ_I(_chmod(FN, _S_IREAD), 0);
    CHECK_EQ_I(_stat64(FN, &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IWRITE, 0);
    CHECK_EQ_I(st.st_mode & _S_IREAD, _S_IREAD);
    CHECK((GetFileAttributesA(FN) & FILE_ATTRIBUTE_READONLY) != 0);
    CHECK_EQ_I(_chmod(FN, _S_IREAD | _S_IWRITE), 0);
    CHECK_EQ_I(_stat64(FN, &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IWRITE, _S_IWRITE);
    CHECK_EQ_I(st.st_mode & _S_IREAD, _S_IREAD);
    CHECK_EQ_I(_unlink(FN), 0);
    rm(FN);
}
int main(void) { tc_begin(); TC_RUN(readonly_toggle); return tc_end(); }
