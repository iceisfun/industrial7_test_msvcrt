/* TC
 * id:    chr_neg_ungetc_write_only
 * area:  stdio_char
 * kind:  negative
 * funcs: ungetc fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.7.10; MS docs ungetc
 * note:  PROBE: write-only stream rejects pushback with EOF (no handler call asserted).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_ungetc_write_only_a" TCV_S(TC_CRT_VER) ".tmp"
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
    FILE *f; volatile int c = 'a';
    put_file("", 0);
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ungetc(c, f), EOF);
    fclose(f);
    CHECK_EQ_U(file_size(), 0);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(write_only); DeleteFileA(FN); return tc_end(); }
