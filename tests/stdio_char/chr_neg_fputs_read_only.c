/* TC
 * id:    chr_neg_fputs_read_only
 * area:  stdio_char
 * kind:  negative
 * funcs: fputs ferror fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs fputs (returns EOF on error); wrong-mode use not explicitly documented
 * note:  fputs on read-only stream fails; file unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fputs_read_only_a" TCV_S(TC_CRT_VER) ".tmp"
static void put_file(const char *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { if (n) WriteFile(h, data, n, &w, NULL); CloseHandle(h); }
}
static DWORD file_size(void)
{
    DWORD s = 0xffffffff; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { s = GetFileSize(h, NULL); CloseHandle(h); }
    return s;
}

TC_CASE(read_only)
{
    FILE *f; const char * volatile s = "hello";
    put_file("abc", 3);
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fputs(s, f), EOF);
    CHECK(ferror(f) != 0);
    fclose(f);
    CHECK_EQ_U(file_size(), 3);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(read_only); DeleteFileA(FN); return tc_end(); }
