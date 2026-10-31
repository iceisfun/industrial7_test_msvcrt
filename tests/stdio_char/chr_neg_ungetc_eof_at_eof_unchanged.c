/* TC
 * id:    chr_neg_ungetc_eof_at_eof_unchanged
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc fgetc feof fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs ungetc ("If c is EOF, the operation fails and the input stream is unchanged"); C11 7.21.7.10p4
 * note:  ungetc(EOF) at true end of file fails and leaves the EOF indicator set. Contrast case: a valid ungetc clears it.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_ungetc_eof_at_eof_unchanged_" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(ungetc_eof_unchanged)
{
    FILE *f; put_file("ab", 2);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc(EOF, f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(ferror(f) == 0);
    fclose(f);
}
TC_CASE(valid_ungetc_clears_eof)
{
    FILE *f; put_file("ab", 2);
    f = fopen(FN, "rb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc('b', f), 'b');
    CHECK(feof(f) == 0);
    CHECK_EQ_I(fgetc(f), 'b');
    fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(ungetc_eof_unchanged); TC_RUN(valid_ungetc_clears_eof); remove(FN); return tc_end(); }
