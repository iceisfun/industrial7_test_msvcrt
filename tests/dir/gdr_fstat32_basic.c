/* TC
 * id:    gdr_fstat32_basic
 * area:  dir
 * funcs: _fstat32 _fstat32i64 _open _write _close _commit _dup
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _fstat, _fstat32, _fstat32i64 (st_size, st_mode)
 */
#include "tc.h"
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>

#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#define FN "tc_gdr_fstat32_basic" TC_S(TC_CRT_VER) ".tmp"
static void rm(void) { SetFileAttributesA(FN, FILE_ATTRIBUTE_NORMAL); DeleteFileA(FN); }

TC_CASE(regular_and_grow)
{
    struct _stat32 st; struct _stat32i64 s2; int fd;
    rm();
    fd = _open(FN, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "0123456789", 10), 10);
    CHECK_EQ_I(_commit(fd), 0);
    memset(&st, 0x5a, sizeof st);
    CHECK_EQ_I(_fstat32(fd, &st), 0);
    CHECK_EQ_I(st.st_size, 10);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFREG);
    memset(&s2, 0x5a, sizeof s2);
    CHECK_EQ_I(_fstat32i64(fd, &s2), 0);
    CHECK_EQ_I(s2.st_size, 10);
    CHECK_EQ_I(s2.st_mode & _S_IFMT, _S_IFREG);
    CHECK_EQ_I(_write(fd, "abcde", 5), 5);
    CHECK_EQ_I(_commit(fd), 0);
    CHECK_EQ_I(_fstat32(fd, &st), 0);
    CHECK_EQ_I(st.st_size, 15);
    CHECK_EQ_I(_fstat32i64(fd, &s2), 0);
    CHECK_EQ_I(s2.st_size, 15);
    _close(fd);
    rm();
}
TC_CASE(dup_same_size)
{
    struct _stat32 a, b; int fd, d;
    rm();
    fd = _open(FN, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0);
    if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "xyz", 3), 3);
    CHECK_EQ_I(_commit(fd), 0);
    d = _dup(fd);
    CHECK(d >= 0);
    if (d >= 0) {
        CHECK_EQ_I(_fstat32(fd, &a), 0);
        CHECK_EQ_I(_fstat32(d, &b), 0);
        CHECK_EQ_I(a.st_size, 3); CHECK_EQ_I(b.st_size, 3);
        _close(d);
    }
    _close(fd);
    rm();
}
TC_CASE(nul_device)
{
    struct _stat32 st; struct _stat32i64 s2; int fd = _open("NUL", _O_RDWR | _O_BINARY);
    CHECK(fd >= 0);
    if (fd < 0) return;
    memset(&st, 0x5a, sizeof st);
    CHECK_EQ_I(_fstat32(fd, &st), 0);
    CHECK_EQ_I(st.st_mode & _S_IFMT, _S_IFCHR);
    CHECK_EQ_I(st.st_size, 0);
    memset(&s2, 0x5a, sizeof s2);
    CHECK_EQ_I(_fstat32i64(fd, &s2), 0);
    CHECK_EQ_I(s2.st_mode & _S_IFMT, _S_IFCHR);
    CHECK_EQ_I(s2.st_size, 0);
    _close(fd);
}
int main(void) { tc_begin(); rm(); TC_RUN(regular_and_grow); TC_RUN(dup_same_size); TC_RUN(nul_device); rm(); return tc_end(); }
