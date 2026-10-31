/* TC
 * id:    chr_neg_fgetc_fgets_append_only
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fgets fgetwc fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs fopen ("a": open for writing at end of file), fgetc (EOF = error or end of file; use ferror/feof)
 * note:  Reading from a stream opened "a" (write-only) fails (EOF/NULL/WEOF). The error-indicator claim lives in chr_neg_probe_append_read_sets_error. Wrong-mode reads are not
 *        explicitly specified, hence inferred. invalid-parameter handler is installed in case a CRT reports it.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgetc_fgets_append_only_" TCV_S(TC_CRT_VER) ".tmp"
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

static void chk_unchanged(void)
{
    char b[16]; long n = get_file(b, sizeof b);
    CHECK_EQ_I(n, 3); if (n == 3) CHECK_MEM(b, "abc", 3);
}
TC_CASE(fgetc_append)
{
    FILE *f; put_file("abc", 3);
    f = fopen(FN, "a"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fgetc(f), EOF);
    fclose(f);
}
TC_CASE(fgets_append)
{
    FILE *f; char b[8]; put_file("abc", 3);
    f = fopen(FN, "a"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(b, 8, f) == NULL);
    fclose(f);
}
TC_CASE(fgetwc_append)
{
    FILE *f; put_file("abc", 3);
    f = fopen(FN, "a"); CHECK(f != NULL); if (!f) return;
    CHECK(fgetwc(f) == WEOF);
    fclose(f);
}
TC_CASE(content_unchanged) { chk_unchanged(); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fgetc_append); TC_RUN(fgets_append); TC_RUN(fgetwc_append); TC_RUN(content_unchanged);
    remove(FN); return tc_end();
}
