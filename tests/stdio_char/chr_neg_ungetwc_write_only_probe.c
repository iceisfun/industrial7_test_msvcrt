/* TC
 * id:    chr_neg_ungetwc_write_only_probe
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetwc fputwc fopen fclose
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs ungetwc; C11 7.29.3.10
 * note:  PROBE: ungetwc on a "w" (write-only, ANSI text) stream returns WEOF and does not disturb later output "AB".
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_ungetwc_write_only_probe_" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(unget_on_write_stream)
{
    FILE *f; char b[16]; long n;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK(ungetwc(L'x', f) == WEOF);
    CHECK(fputwc(L'A', f) == L'A');
    CHECK(fputwc(L'B', f) == L'B');
    CHECK(fclose(f) == 0);
    n = get_file(b, sizeof b);
    CHECK_EQ_I(n, 2); if (n == 2) CHECK_MEM(b, "AB", 2);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(unget_on_write_stream); remove(FN); return tc_end(); }
