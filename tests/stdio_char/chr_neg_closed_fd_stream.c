/* TC
 * id:    chr_neg_closed_fd_stream
 * area:  stdio_char
 * funcs: fgetc fputc fflush fclose fopen _close _fileno
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fgetc, fflush, _close
 * note:  PROBE: closing the descriptor underneath a FILE. Read side: fgetc returns EOF and sets the error indicator.
 *        Write side: fputc of a byte is absorbed by the buffer, fflush then fails with EOF and errno EBADF.
 *        The invalid-parameter handler is installed so any invalid-fd report is harmless; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>

#define FN "tc_chr_neg_closed_fd_stream.tmp"
#define FN2 "tc_chr_neg_closed_fd_stream2.tmp"
static void mk(const char *name, const char *data, size_t n)
{
    FILE *f = fopen(name, "wb");
    if (f) { if (n) fwrite(data, 1, n, f); fclose(f); }
}
TC_CASE(read_side)
{
    FILE *f; int r;
    mk(FN, "hello", 5);
    f = fopen(FN, "rb");
    CHECK(f != NULL);
    if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    errno = 0;
    r = (fgetc)(f);
    CHECK_EQ_I(r, EOF);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(write_side)
{
    FILE *f; int r;
    f = fopen(FN2, "wb");
    CHECK(f != NULL);
    if (!f) { remove(FN2); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    r = (fputc)('x', f);
    CHECK_EQ_I(r, 'x');
    errno = 0;
    r = fflush(f);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(FN2);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(read_side); TC_RUN(write_side);
    return tc_end();
}
