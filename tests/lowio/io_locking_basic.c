/* TC
 * id:    io_locking_basic
 * area:  lowio
 * funcs: _locking _open _close _lseek _write
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _locking (locks/unlocks bytes starting at the current file position; _LK_LOCK, _LK_NBLCK, _LK_UNLCK)
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/locking.h>

static const char *volatile fn = "tc_io_locking_basic_" TC_S(TC_CRT_VER) ".tmp";
static int mk(void)
{
    char buf[100]; int fd, i;
    for (i = 0; i < 100; i++) buf[i] = 'a';
    DeleteFileA(fn);
    fd = _open(fn, _O_CREAT | _O_RDWR | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) _write(fd, buf, 100);
    return fd;
}

TC_CASE(nblck_then_unlck)
{
    int fd = mk();
    CHECK(fd >= 0);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_locking(fd, _LK_NBLCK, 10), 0);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 0);
    CHECK_EQ_I(_locking(fd, _LK_UNLCK, 10), 0);
    _close(fd); DeleteFileA(fn);
}
TC_CASE(lock_at_position)
{
    int fd = mk();
    CHECK(fd >= 0);
    CHECK_EQ_I(_lseek(fd, 50, SEEK_SET), 50);
    CHECK_EQ_I(_locking(fd, _LK_NBLCK, 10), 0);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 50);   /* file pointer is restored */
    CHECK_EQ_I(_locking(fd, _LK_UNLCK, 10), 0);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_CUR), 50);
    _close(fd); DeleteFileA(fn);
}
TC_CASE(lk_lock_free_region)
{
    int fd = mk();
    CHECK(fd >= 0);
    CHECK_EQ_I(_lseek(fd, 20, SEEK_SET), 20);
    CHECK_EQ_I(_locking(fd, _LK_LOCK, 5), 0);
    CHECK_EQ_I(_lseek(fd, 20, SEEK_SET), 20);
    CHECK_EQ_I(_locking(fd, _LK_UNLCK, 5), 0);
    _close(fd); DeleteFileA(fn);
}
TC_CASE(lock_past_eof)
{
    int fd = mk();
    CHECK(fd >= 0);
    CHECK_EQ_I(_lseek(fd, 1000, SEEK_SET), 1000);
    CHECK_EQ_I(_locking(fd, _LK_NBLCK, 4), 0);
    CHECK_EQ_I(_locking(fd, _LK_UNLCK, 4), 0);
    _close(fd); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(nblck_then_unlck); TC_RUN(lock_at_position); TC_RUN(lk_lock_free_region); TC_RUN(lock_past_eof); return tc_end(); }
