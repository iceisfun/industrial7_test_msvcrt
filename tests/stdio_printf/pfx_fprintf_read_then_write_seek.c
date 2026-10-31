/* TC
 * id:    pfx_fprintf_read_then_write_seek
 * area:  stdio_printf
 * funcs: fprintf fgetc fopen fclose fseek rewind
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3p7 (input followed by output requires an intervening fseek/fflush/rewind unless EOF was reached)
 * note:  Well-defined companion of pfx_neg_probe_fprintf_after_read_no_seek: write after read succeeds with an intervening fseek, and after the read hit EOF.
 */
#include "tc.h"
#include <stdio.h>
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
static int rd(const char *p, char *b, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); DWORD got = 0;
    if (h == INVALID_HANDLE_VALUE) { b[0] = 0; return -1; }
    ReadFile(h, b, (DWORD)(n - 1), &got, NULL); CloseHandle(h); b[got] = 0; return (int)got;
}
static void wr(const char *p, const char *d, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, (DWORD)n, &w, NULL); CloseHandle(h); }
}

#define TMP "tc_pfx_fprintf_read_then_write_seek_" TCV ".tmp"

TC_CASE(with_seek)
{
    FILE *f = fopen(TMP, "w+"); char b[16]; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abcdef"), 6);
    rewind(f);
    CHECK_EQ_I(fgetc(f), 'a');
    CHECK_EQ_I(fseek(f, 0, SEEK_CUR), 0);
    CHECK_EQ_I(fprintf(f, "X"), 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 6); CHECK_STR(b, "aXcdef");
    DeleteFileA(TMP);
}
TC_CASE(after_eof)
{
    FILE *f = fopen(TMP, "w+"); char b[16]; int c, n = 0; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abcdef"), 6);
    rewind(f);
    while ((c = fgetc(f)) != EOF && n < 20) n++;
    CHECK_EQ_I(n, 6);
    CHECK_EQ_I(fprintf(f, "X"), 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 7); CHECK_STR(b, "abcdefX");
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    TC_RUN(with_seek); TC_RUN(after_eof);
    return tc_end();
}
