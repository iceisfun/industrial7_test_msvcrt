/* TC
 * id:    ucr_neg_iob_func_fileno_after_close
 * area:  ucrt_only
 * kind:  negative
 * funcs: __acrt_iob_func _fileno _close _open _dup2 setvbuf fputs fflush
 * crt:   win10
 * tier:  p0
 * conf:  probe
 * ref:   MS docs _close, fflush (returns EOF on write error), _write (EBADF for invalid fd), _fileno
 * note:  PROBES the fflush failure path (EOF and errno EBADF coming from _write on a closed fd; errno for fflush is not documented).
 *        Child redirects fd 1 to a temp file (_dup2), gives stdout a full buffer, buffers a string, _close(1), then fflush must fail
 *        with EOF and EBADF; _fileno(stdout) still reports 1. Child exits 77 when everything held, otherwise 70+n. temp file
 *        tc_ucr_neg_iob_func_fileno_after_close.tmp
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

#define TMP "tc_ucr_neg_iob_func_fileno_after_close.tmp"
static char sbuf[256];

TC_CASE(child_flush_fails)
{
    DWORD code = 99;
    CHECK(tc_spawn_self("closed1", &code, 20000));
    CHECK_EQ_U(code, 77);
    DeleteFileA(TMP);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    if (tc_child_name()) {
        FILE *so = __acrt_iob_func(1);
        int fd, r1, r2, e;
        fd = _open(TMP, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
        if (fd < 0) return 60;
        if (_dup2(fd, 1) != 0) return 61;
        if (fd != 1) _close(fd);
        if (setvbuf(so, sbuf, _IOFBF, sizeof sbuf) != 0) return 62;
        if (_close(1) != 0) return 63;
        errno = 0;
        r1 = fputs("hello", so);
        r2 = fflush(so);
        e = errno;
        if (r1 != EOF && r2 != EOF) return 71;
        if (e != EBADF) return 72;
        if (_fileno(so) != 1) return 73;
        return 77;
    }
    TC_RUN(child_flush_fails);
    return tc_end();
}
