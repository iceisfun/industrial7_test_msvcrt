/* TC
 * id:    chr_neg_fgetws_fputws_closed_fd_probe
 * area:  stdio_char
 * funcs: fgetws fputws fgetwc fputwc _close _fileno fflush fopen fclose ferror
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs fgetwc, fputwc, _close
 * note:  PROBE: wide I/O on a FILE whose descriptor was closed underneath it. Read side: fgetwc returns WEOF and fgetws
 *        returns NULL with the error indicator set. Write side: the buffered write is accepted, fflush then fails with EOF.
 *        Invalid-parameter handler installed; its count is not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
#include <wchar.h>
#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_chr_neg_fgetws_fputws_closed_fd_probe_" TCS(TC_CRT_VER) ".tmp"

static void mk(void)
{
    FILE *f = fopen(FN, "wb");
    if (f) { fwrite("hello world", 1, 11, f); fclose(f); }
}
TC_CASE(fgetwc_read)
{
    FILE *f; wint_t c;
    mk(); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    c = fgetwc(f);
    CHECK_EQ_U(c, WEOF);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(fgetws_read)
{
    FILE *f; wchar_t buf[8]; wchar_t *p;
    mk(); f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    p = fgetws(buf, 8, f);
    CHECK(p == NULL);
    CHECK(ferror(f) != 0);
    fclose(f); remove(FN);
}
TC_CASE(fputwc_then_fflush)
{
    FILE *f; wint_t r;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    r = fputwc(L'x', f);
    CHECK_EQ_U(r, L'x');
    CHECK_EQ_I(fflush(f), EOF);
    fclose(f); remove(FN);
}
TC_CASE(fputws_then_fflush)
{
    FILE *f; int r;
    f = fopen(FN, "wb"); CHECK(f != NULL); if (!f) { remove(FN); return; }
    CHECK_EQ_I(_close(_fileno(f)), 0);
    r = fputws(L"abc", f);
    CHECK(r >= 0);
    CHECK_EQ_I(fflush(f), EOF);
    fclose(f); remove(FN);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fgetwc_read); TC_RUN(fgetws_read); TC_RUN(fputwc_then_fflush); TC_RUN(fputws_then_fflush);
    return tc_end();
}
