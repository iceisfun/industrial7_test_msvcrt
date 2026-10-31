/* TC
 * id:    chr_neg_fgets_no_overrun
 * area:  stdio_char
 * kind:  negative
 * funcs: fgets fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.2 (at most n-1 chars read, NUL appended); MS docs fgets
 * note:  Guard bytes after the buffer limit must stay intact; result always NUL-terminated.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgets_no_overrun_" TCV_S(TC_CRT_VER) ".tmp"
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

#define GUARD 0xa5
static void fillg(unsigned char *b, int n) { int i; for (i = 0; i < n; i++) b[i] = GUARD; }
static int guards_ok(const unsigned char *b, int from, int to) { int i; for (i = from; i < to; i++) if (b[i] != GUARD) return 0; return 1; }

TC_CASE(n5_long_line)
{
    unsigned char b[16]; char big[101]; FILE *f; int i;
    for (i = 0; i < 99; i++) big[i] = (char)('a' + i % 26);
    big[99] = '\n'; big[100] = 0;
    put_file(big, 100);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    fillg(b, 16);
    CHECK(fgets((char *)b, 5, f) == (char *)b);
    CHECK_MEM(b, "abcd", 5);
    CHECK(guards_ok(b, 5, 16));
    fclose(f);
}
TC_CASE(n3_newline_remains)
{
    unsigned char b[8]; FILE *f;
    put_file("ab\n", 3);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    fillg(b, 8);
    CHECK(fgets((char *)b, 3, f) == (char *)b);
    CHECK_MEM(b, "ab", 3);
    CHECK(guards_ok(b, 3, 8));
    fillg(b, 8);
    CHECK(fgets((char *)b, 3, f) == (char *)b);
    CHECK_MEM(b, "\n", 2);
    CHECK(guards_ok(b, 2, 8));
    fclose(f);
}
TC_CASE(text_crlf_boundary)
{
    unsigned char b[8]; FILE *f;
    put_file("abc\r\ndef\r\n", 10);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    fillg(b, 8);
    CHECK(fgets((char *)b, 4, f) == (char *)b);     /* "abc" */
    CHECK_MEM(b, "abc", 4);
    CHECK(guards_ok(b, 4, 8));
    fillg(b, 8);
    CHECK(fgets((char *)b, 4, f) == (char *)b);     /* CRLF translated to "\n" */
    CHECK_MEM(b, "\n", 2);
    CHECK(guards_ok(b, 2, 8));
    fillg(b, 8);
    CHECK(fgets((char *)b, 4, f) == (char *)b);     /* "def" */
    CHECK_MEM(b, "def", 4);
    CHECK(guards_ok(b, 4, 8));
    fclose(f);
}
int main(void) { tc_begin(); TC_RUN(n5_long_line); TC_RUN(n3_newline_remains); TC_RUN(text_crlf_boundary); remove(FN); return tc_end(); }
