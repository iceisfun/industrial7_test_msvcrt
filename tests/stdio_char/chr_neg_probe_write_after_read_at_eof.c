/* TC
 * id:    chr_neg_probe_write_after_read_at_eof
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc fclose fopen
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.5.3p7 (output after input is allowed if the input hit end-of-file); MS _flsbuf accepts when _IOEOF set
 * note:  PROBE: on "r+" containing "ab", after reading to EOF an fputc without seek succeeds and appends: file becomes "abZ".
 *        Contrast with chr_neg_probe_write_after_read_mid_file.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_probe_write_after_read_at_eof_" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(write_after_eof)
{
    FILE *f; char b[16]; long n;
    put_file("ab", 2);
    f = fopen(FN, "r+"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(fputc('Z', f), 'Z');
    CHECK(fclose(f) == 0);
    n = get_file(b, sizeof b);
    CHECK_EQ_I(n, 3); if (n == 3) CHECK_MEM(b, "abZ", 3);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(write_after_eof); remove(FN); return tc_end(); }
