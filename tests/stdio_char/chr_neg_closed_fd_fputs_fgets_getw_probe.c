/* TC
 * id:    chr_neg_closed_fd_fputs_fgets_getw_probe
 * area:  stdio_char
 * funcs: fputs fgets _getw _putw fflush _close _fileno fopen fclose ferror
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _close, fflush, _getw, fgets
 * note:  PROBE (_putw is lenient: value written, or EOF with ferror, since the first buffer allocation may already hit the bad fd): the descriptor of a FILE is closed with _close(_fileno(f)). Write side: fputs/_putw are absorbed by the
 *        buffer, fflush then fails with EOF and errno EBADF. Read side: fgets returns NULL and _getw returns EOF, both
 *        with the error indicator set. Invalid-parameter handler installed; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <errno.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_closed_fd_fputs_fgets_getw_probe_" TCS(TC_CRT_VER) ".tmp"

static void mk(void)
{
    FILE *f = fopen(FN, "wb");
    if (f) { fwrite("hello world", 1, 11, f); fclose(f); }
}
TC_CASE(fgets_read)
{
    FILE *f; char buf[16]; char *p;
    mk(); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    p = fgets(buf, (int)sizeof buf, f);
    CHECK(p == NULL);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(getw_read)
{
    FILE *f; int r;
    mk(); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    r = _getw(f);
    CHECK_EQ_I(r, EOF);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(fputs_then_fflush)
{
    FILE *f; int r;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    r = fputs("abc", f);
    CHECK(r >= 0);
    errno = 0;
    r = fflush(f);
    CHECK_EQ_I(r, EOF);
    CHECK_EQ_I(errno, EBADF);
    fclose(f); remove(FN);
}
TC_CASE(putw_then_fflush)
{
    FILE *f; int r;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    r = _putw(0x01020304, f);
    CHECK(r == 0x01020304 || r == EOF);
    if (r == 0x01020304) {          /* absorbed by the buffer: the failure surfaces at flush */
        errno = 0;
        CHECK_EQ_I(fflush(f), EOF);
        CHECK_EQ_I(errno, EBADF);
    } else CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fgets_read); TC_RUN(getw_read); TC_RUN(fputs_then_fflush); TC_RUN(putw_then_fflush);
    return tc_end();
}
