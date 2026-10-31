/* TC
 * id:    chr_neg_fgetws_no_overrun
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetws fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fgetws (reads at most n-1 wide chars, appends L'\0'); C11 7.29.3.2
 * note:  fgetws must never store more than n wide characters. Canary wchar_t values after the buffer limit must stay intact.
 *        Text-mode ANSI stream, ASCII data, so each byte maps to one wchar_t.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgetws_no_overrun_" TCV_S(TC_CRT_VER) ".tmp"
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

#define CAN ((wchar_t)0x5a5a)
static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = CAN; }

TC_CASE(n4_long_line)
{
    wchar_t b[8]; FILE *f; int i;
    put_file("abcdefghijklmnopqrstuvwxyz\n", 27);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    fill(b, 8);
    CHECK(fgetws(b, 4, f) == b);
    CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[1], L'b'); CHECK_EQ_I(b[2], L'c'); CHECK_EQ_I(b[3], 0);
    for (i = 4; i < 8; i++) CHECK_EQ_I(b[i], CAN);
    fclose(f);
}
TC_CASE(n2_short_line)
{
    wchar_t b[6]; FILE *f; int i;
    put_file("abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    fill(b, 6);
    CHECK(fgetws(b, 2, f) == b);
    CHECK_EQ_I(b[0], L'a'); CHECK_EQ_I(b[1], 0);
    for (i = 2; i < 6; i++) CHECK_EQ_I(b[i], CAN);
    fclose(f);
}
TC_CASE(rest_still_readable)
{
    wchar_t b[4], c[16]; FILE *f;
    put_file("abcdef\n", 7);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetws(b, 4, f) == b);
    CHECK_WSTR(b, L"abc");
    CHECK(fgetws(c, 16, f) == c);
    CHECK_WSTR(c, L"def\n");
    CHECK(fgetws(c, 16, f) == NULL);
    CHECK(feof(f) != 0);
    fclose(f);
}
int main(void) { tc_begin(); TC_RUN(n4_long_line); TC_RUN(n2_short_line); TC_RUN(rest_still_readable); remove(FN); return tc_end(); }
