/* TC
 * id:    chr_neg_wrong_mode_no_handler_probe
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc fgets fputs fgetwc fputwc ungetc _getw _putw fopen fclose
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs parameter validation (handler is for NULL/invalid arguments, not wrong-mode streams)
 * note:  PROBE: wrong-direction I/O returns failure without calling the invalid-parameter handler (the stream argument itself is valid).
 *        errno values are not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_wrong_mode_no_handler_probe_" TCV_S(TC_CRT_VER) ".tmp"
static void put_file(const char *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (n) WriteFile(h, data, n, &w, NULL); CloseHandle(h); }
}
/* returns size (or -1) and copies up to cap bytes into out */
static long get_file(char *out, DWORD cap)
{
    DWORD r = 0, s; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    s = GetFileSize(h, NULL);
    if (s <= cap) ReadFile(h, out, s, &r, NULL);
    CloseHandle(h);
    return (long)s;
}

TC_CASE(read_on_write_stream)
{
    FILE *f = fopen(FN, "w"); char b[8]; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    CHECK_EQ_I(fgetc(f), EOF);          CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK(fgets(b, 8, f) == NULL);      CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK(fgetwc(f) == WEOF);           CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK_EQ_I(ungetc('x', f), EOF);    CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK_EQ_I(_getw(f), EOF);          CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
}
TC_CASE(write_on_read_stream)
{
    FILE *f; put_file("abcdefgh", 8);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0;
    CHECK_EQ_I(fputc('x', f), EOF);     CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK_EQ_I(fputs("xy", f), EOF);    CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK(fputwc(L'x', f) == WEOF);     CHECK_EQ_I(tc_iph_count, 0); clearerr(f);
    CHECK_EQ_I(_putw(1, f), EOF);       CHECK_EQ_I(tc_iph_count, 0);
    fclose(f);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC SKIP no handler API\n"); return tc_end(); }
    TC_RUN(read_on_write_stream); TC_RUN(write_on_read_stream);
    remove(FN); return tc_end();
}
