/* TC
 * id:    io_neg_locking_conflict
 * area:  lowio
 * funcs: _locking _sopen _close _lseek
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _locking (errno EACCES: locking violation, file already locked or unlocked; _LK_NBLCK/_LK_NBRLCK do not retry)
 */
#include "tc.h"
#include <stdio.h>
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <share.h>
#include <sys/stat.h>
#include <sys/locking.h>

static const char *volatile fn = "tc_io_neg_locking_conflict_" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(second_handle_blocked)
{
    int a, b;
    DeleteFileA(fn);
    a = _sopen(fn, _O_CREAT | _O_RDWR | _O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
    b = _sopen(fn, _O_RDWR | _O_BINARY, _SH_DENYNO);
    CHECK(a >= 0 && b >= 0);
    if (a < 0 || b < 0) { if (a >= 0) _close(a); if (b >= 0) _close(b); DeleteFileA(fn); return; }
    CHECK_EQ_I(_lseek(a, 0, SEEK_SET), 0);
    CHECK_EQ_I(_locking(a, _LK_NBLCK, 10), 0);

    CHECK_EQ_I(_lseek(b, 0, SEEK_SET), 0);
    errno = 0;
    CHECK_EQ_I(_locking(b, _LK_NBLCK, 10), -1);
    CHECK_EQ_I(errno, EACCES);

    CHECK_EQ_I(_lseek(b, 5, SEEK_SET), 5);        /* overlapping region */
    errno = 0;
    CHECK_EQ_I(_locking(b, _LK_NBLCK, 10), -1);
    CHECK_EQ_I(errno, EACCES);

    CHECK_EQ_I(_lseek(b, 0, SEEK_SET), 0);
    errno = 0;
    CHECK_EQ_I(_locking(b, _LK_NBRLCK, 10), -1);
    CHECK_EQ_I(errno, EACCES);

    CHECK_EQ_I(_lseek(b, 10, SEEK_SET), 10);      /* adjacent, disjoint region is free */
    CHECK_EQ_I(_locking(b, _LK_NBLCK, 5), 0);
    CHECK_EQ_I(_lseek(b, 10, SEEK_SET), 10);
    CHECK_EQ_I(_locking(b, _LK_UNLCK, 5), 0);

    CHECK_EQ_I(_lseek(a, 0, SEEK_SET), 0);
    CHECK_EQ_I(_locking(a, _LK_UNLCK, 10), 0);
    CHECK_EQ_I(_lseek(b, 0, SEEK_SET), 0);
    CHECK_EQ_I(_locking(b, _LK_NBLCK, 10), 0);    /* now free */
    CHECK_EQ_I(_lseek(b, 0, SEEK_SET), 0);
    CHECK_EQ_I(_locking(b, _LK_UNLCK, 10), 0);
    _close(a); _close(b); DeleteFileA(fn);
}
int main(void) { tc_begin(); TC_RUN(second_handle_blocked); return tc_end(); }
