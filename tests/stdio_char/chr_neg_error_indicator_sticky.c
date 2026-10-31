/* TC
 * id:    chr_neg_error_indicator_sticky
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fputc ferror clearerr feof fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs clearerr (error indicator stays set until clearerr/rewind), ferror; C11 7.21.10.1
 * note:  The error indicator set by a wrong-mode fgetc/fputc persists across later calls until clearerr. That the wrong-mode call
 *        sets the indicator at all is inferred (see chr_neg_fgetc_write_only); the persistence is the documented part.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_error_indicator_sticky_" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(fgetc_on_write_stream)
{
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(ferror(f) != 0);
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(ferror(f) != 0);
    CHECK(feof(f) == 0);
    clearerr(f);
    CHECK(ferror(f) == 0);
    CHECK(feof(f) == 0);
    fclose(f);
}
TC_CASE(fputc_on_read_stream)
{
    FILE *f; put_file("abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputc('x', f), EOF);
    CHECK(ferror(f) != 0);
    CHECK(feof(f) == 0);
    CHECK_EQ_I(fputc('y', f), EOF);
    CHECK(ferror(f) != 0);
    CHECK(feof(f) == 0);
    clearerr(f);
    CHECK(ferror(f) == 0);
    CHECK(feof(f) == 0);
    fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fgetc_on_write_stream); TC_RUN(fputc_on_read_stream); remove(FN); return tc_end(); }
