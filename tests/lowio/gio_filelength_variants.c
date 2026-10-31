/* TC
 * id:    gio_filelength_variants
 * area:  lowio
 * funcs: _filelength _filelengthi64 _open _write _chsize _close
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _filelength/_filelengthi64: length in bytes of the file
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
static void rm(const char *n) { SetFileAttributesA(n, FILE_ATTRIBUTE_NORMAL); DeleteFileA(n); }
#define N "tc_gio_filelength_variants.tmp"
TC_CASE(empty_and_write)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_filelength(fd), 0);
    CHECK_EQ_I(_write(fd, "hello", 5), 5);
    CHECK_EQ_I(_filelength(fd), 5);
    CHECK_EQ_I(_filelengthi64(fd), _filelength(fd));
    _close(fd); rm(N);
}
TC_CASE(after_chsize)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "hello", 5), 5);
    CHECK_EQ_I(_chsize(fd, 100), 0);
    CHECK_EQ_I(_filelength(fd), 100);
    CHECK_EQ_I(_filelengthi64(fd), 100);
    CHECK_EQ_I(_chsize(fd, 3), 0);
    CHECK_EQ_I(_filelength(fd), 3);
    CHECK_EQ_I(_filelengthi64(fd), 3);
    _close(fd); rm(N);
}
TC_CASE(text_mode_raw_bytes)
{
    int fd; rm(N);
    fd = _open(N, _O_RDWR | _O_CREAT | _O_TRUNC | _O_TEXT, _S_IREAD | _S_IWRITE);
    CHECK(fd >= 0); if (fd < 0) return;
    CHECK_EQ_I(_write(fd, "a\nb", 3), 3);
    CHECK_EQ_I(_filelength(fd), 4);
    CHECK_EQ_I(_filelengthi64(fd), 4);
    _close(fd); rm(N);
}
int main(void)
{
    tc_begin();
    TC_RUN(empty_and_write); TC_RUN(after_chsize); TC_RUN(text_mode_raw_bytes);
    rm(N);
    return tc_end();
}
