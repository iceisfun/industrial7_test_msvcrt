/* TC
 * id:    chr_neg_fputws_ansi_unrepresentable_probe
 * area:  stdio_char
 * kind:  negative
 * funcs: fputws fopen fclose fflush
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.29.3.4/7.29.3.3 (encoding error: EILSEQ); MS docs fputws (returns WEOF on error)
 * note:  PROBE: L"A\x263A"L"B" on an ANSI text stream in the C locale cannot be fully converted. Expect failure (WEOF, or -1 as int)
 *        with errno EILSEQ; separately, nothing after the bad character reaches the file (length <= 2).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fputws_ansi_unrepresentable_probe_" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(fails_with_eilseq)
{
    FILE *f; int r; static const wchar_t s[] = { L'A', 0x263A, L'B', 0 };
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0; r = fputws(s, f);
    CHECK(r == (int)WEOF || r == -1);
    CHECK_EQ_I(errno, EILSEQ);
    fclose(f);
}
TC_CASE(nothing_after_bad_char)
{
    FILE *f; char b[16]; long n; static const wchar_t s[] = { L'A', 0x263A, L'B', 0 };
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    (void)fputws(s, f);
    fflush(f);
    fclose(f);
    n = get_file(b, sizeof b);
    CHECK(n >= 0 && n <= 2);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fails_with_eilseq); TC_RUN(nothing_after_bad_char); remove(FN); return tc_end(); }
