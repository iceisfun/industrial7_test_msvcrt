/* TC
 * id:    chr_neg_fgets_write_only
 * area:  stdio_char
 * kind:  negative
 * funcs: fgets ferror fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  inferred
 * ref:   MS docs fgets (returns NULL on error or EOF); wrong-mode use not explicitly documented
 * note:  Buffer contents after failure are intentionally not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgets_write_only_a" TCV_S(TC_CRT_VER) ".tmp"
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

TC_CASE(write_only)
{
    FILE *f; char buf[16]; volatile int n = 16;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK(fgets(buf, n, f) == NULL);
    CHECK(ferror(f) != 0);
    fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(write_only); DeleteFileA(FN); return tc_end(); }
