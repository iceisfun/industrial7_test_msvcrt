/* TC
 * id:    io_neg_stat64_empty_path_probe
 * area:  lowio
 * funcs: _stat64 _wstat64
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _stat (not explicit about "")
 * note:  PROBES: _stat64("") and _wstat64(L"") return -1 with errno ENOENT (and do not call the invalid-parameter handler).
 *        Only the return value and errno are asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }

TC_CASE(empty)
{
    struct _stat64 st;
    volatile char e = 0; char buf[2]; wchar_t wbuf[2];
    buf[0] = e; buf[1] = 0; wbuf[0] = e; wbuf[1] = 0;
    CHECK(tc_iph_install());
    errno = 0;
    CHECK_EQ_I(_stat64(buf, &st), -1);
    CHECK_EQ_I(errno, ENOENT);
    errno = 0;
    CHECK_EQ_I(_wstat64(wbuf, &st), -1);
    CHECK_EQ_I(errno, ENOENT);
}
int main(void) { tc_begin(); TC_RUN(empty); return tc_end(); }
