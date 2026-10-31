/* TC
 * id:    gio_chsize_extend_zerofill
 * area:  lowio
 * funcs: _chsize _open _read _write _close _unlink _filelength
 * procs: _chsize_s
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _chsize, _chsize_s: if the file is extended, null characters ('\0') are appended; if truncated, data is lost
 * note:  _chsize_s is resolved dynamically (not exported by msvcrt.dll / msvcr71-); skipped where absent.
 */
#include "tc.h"
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define TCV TCS(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef int (__cdecl *chsize_s_t)(int, __int64);
static chsize_s_t p_chsize_s;
static const char *volatile fn = "tc_gio_chsize_extend_" TCV ".tmp";
#define PM (_S_IREAD | _S_IWRITE)

static int mk(int flags)
{
    int fd; _unlink(fn);
    fd = _open(fn, _O_CREAT | _O_TRUNC | _O_RDWR | flags, PM);
    if (fd >= 0) _write(fd, "abc", 3);
    return fd;
}
static void check_extended(int fd)
{
    unsigned char b[16]; int i;
    for (i = 0; i < 16; i++) b[i] = 0xEE;
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 16), 10);
    CHECK_MEM(b, "abc", 3);
    for (i = 3; i < 10; i++) if (b[i] != 0) { CHECK_EQ_I(b[i], 0); break; }
}
TC_CASE(extend_zero_bytes)
{
    int fd = mk(_O_BINARY); CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_chsize(fd, 10), 0);
    CHECK_EQ_I(_filelength(fd), 10);
    check_extended(fd);
    _close(fd); _unlink(fn);
}
TC_CASE(shrink_keeps_prefix)
{
    unsigned char b[16] = {0};
    int fd = mk(_O_BINARY); CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_chsize(fd, 2), 0);
    CHECK_EQ_I(_filelength(fd), 2);
    CHECK_EQ_I(_lseek(fd, 0, SEEK_SET), 0);
    CHECK_EQ_I(_read(fd, b, 16), 2);
    CHECK_MEM(b, "ab", 2);
    _close(fd); _unlink(fn);
}
TC_CASE(chsize_returns_zero_on_success)
{
    int fd = mk(_O_BINARY); CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_chsize(fd, 3), 0);      /* same size */
    CHECK_EQ_I(_filelength(fd), 3);
    CHECK_EQ_I(_chsize(fd, 0), 0);
    CHECK_EQ_I(_filelength(fd), 0);
    _close(fd); _unlink(fn);
}
TC_CASE(chsize_s_returns_zero)
{
    int fd;
    if (!p_chsize_s) { CHECK(!TC_AT_LEAST(80)); TC_SKIP("_chsize_s not exported"); }
    fd = mk(_O_BINARY); CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(p_chsize_s(fd, 10), 0);
    CHECK_EQ_I(_filelength(fd), 10);
    check_extended(fd);
    CHECK_EQ_I(p_chsize_s(fd, 1), 0);
    CHECK_EQ_I(_filelength(fd), 1);
    _close(fd); _unlink(fn);
}
TC_CASE(extend_in_binary_vs_text_same_length)
{
    int fd = mk(_O_BINARY); CHECK(fd >= 3); if (fd < 0) return;
    CHECK_EQ_I(_chsize(fd, 10), 0);
    CHECK_EQ_I(_filelength(fd), 10);
    _close(fd);
    fd = mk(_O_TEXT); CHECK(fd >= 3); if (fd < 0) { _unlink(fn); return; }
    CHECK_EQ_I(_chsize(fd, 10), 0);
    CHECK_EQ_I(_filelength(fd), 10);
    _close(fd); _unlink(fn);
}
int main(void)
{
    tc_begin();
    p_chsize_s = (chsize_s_t)tc_crt_proc("_chsize_s");
    TC_RUN(extend_zero_bytes); TC_RUN(shrink_keeps_prefix); TC_RUN(chsize_returns_zero_on_success);
    TC_RUN(chsize_s_returns_zero); TC_RUN(extend_in_binary_vs_text_same_length);
    return tc_end();
}
