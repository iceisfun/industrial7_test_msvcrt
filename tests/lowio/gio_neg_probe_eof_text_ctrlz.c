/* TC
 * id:    gio_neg_probe_eof_text_ctrlz
 * area:  lowio
 * funcs: _eof _open _read _write _close
 * crt:   all
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _read (Ctrl+Z is EOF in text mode)
 * note:  PROBES text-mode _read of "ab^Z cd" stops at ^Z returning 2 bytes. Whether _eof is 0 or 1 afterwards is only
 *        recorded (must be non-negative). Binary read of the whole file then _eof must be 1.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
#define N "tc_gio_neg_probe_eof_text_ctrlz.tmp"
static void mk(void)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    if (fd >= 0) { _write(fd, "ab\x1a" " cd", 6); _close(fd); }
}
TC_CASE(ctrlz_midfile_eof_after_read)
{
    char b[16]; int fd, e; mk();
    fd = _open(N, _O_RDONLY | _O_TEXT); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 16), 2);
    CHECK_EQ_I(b[0], 'a'); CHECK_EQ_I(b[1], 'b');
    e = _eof(fd);
    CHECK(e == 0 || e == 1);
    _close(fd);
}
TC_CASE(binary_same_file_not_eof)
{
    char b[16]; int fd; mk();
    fd = _open(N, _O_RDONLY | _O_BINARY); CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_read(fd, b, 3), 3);
    CHECK_EQ_I(_eof(fd), 0);
    CHECK_EQ_I(_read(fd, b, 16), 3);
    CHECK_EQ_I(_eof(fd), 1);
    _close(fd);
}
int main(void) { tc_begin(); TC_RUN(ctrlz_midfile_eof_after_read); TC_RUN(binary_same_file_not_eof); rm(N); return tc_end(); }
