/* TC
 * id:    pfx_probe_neg_fprintf_after_close_fd
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose _fileno _close setvbuf ferror
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _close, fclose
 * note:  Probing EBADF / -1 when writing through a stream whose fd was closed with _close, ferror nonzero afterwards, and fclose returning EOF.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <io.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_fprintf_after_close_fd_" TCV ".tmp"
static const char *volatile g_fmt = "hello %d";

TC_CASE(write_after_close)
{
    FILE *f = fopen(TMP, "w"); int r, fd, c;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    fd = _fileno(f);
    CHECK_EQ_I(_close(fd), 0);
    tc_iph_count = 0; errno = 0;
    r = fprintf(f, g_fmt, 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EBADF);
    CHECK(ferror(f) != 0);
    errno = 0;
    c = fclose(f);
    CHECK_EQ_I(c, EOF);
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(write_after_close);
    return tc_end();
}
