/* TC
 * id:    fio_fdopen_basic
 * area:  stdio_file
 * funcs: _fdopen fclose fwrite fread
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fdopen
 * note:  _open/_close/_fileno are helpers.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

static const char * volatile fn = "tc_fio_fdopen_basic_a.tmp";

TC_CASE(write_read)
{
    int fd = _open(fn, _O_WRONLY | _O_CREAT | _O_TRUNC | _O_BINARY, _S_IREAD | _S_IWRITE);
    FILE *f; char b[8] = {0};
    CHECK(fd >= 0); if (fd < 0) return;
    f = _fdopen(fd, "wb");
    CHECK(f != NULL);
    if (!f) { _close(fd); return; }
    CHECK_EQ_I(_fileno(f), fd);
    CHECK_EQ_U(fwrite("hello", 1, 5, f), 5);
    CHECK_EQ_I(fclose(f), 0);
    f = fopen(fn, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fread(b, 1, 7, f), 5);
    CHECK_STR(b, "hello");
    fclose(f);
}
TC_CASE(read_mode)
{
    int fd = _open(fn, _O_RDONLY | _O_BINARY);
    FILE *f; char b[8] = {0};
    CHECK(fd >= 0); if (fd < 0) return;
    f = _fdopen(fd, "rb");
    CHECK(f != NULL);
    if (!f) { _close(fd); return; }
    CHECK_EQ_U(fread(b, 1, 3, f), 3);
    CHECK_STR(b, "hel");
    CHECK_EQ_I(fclose(f), 0);
}
int main(void)
{
    tc_begin();
    DeleteFileA(fn);
    TC_RUN(write_read); TC_RUN(read_mode);
    DeleteFileA(fn);
    return tc_end();
}
