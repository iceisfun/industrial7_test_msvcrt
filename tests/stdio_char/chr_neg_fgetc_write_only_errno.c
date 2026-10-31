/* TC
 * id:    chr_neg_fgetc_write_only_errno
 * area:  stdio_char
 * kind:  negative
 * funcs: fgetc fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fgetc, errno
 * note:  PROBE: errno is EBADF after fgetc on a write-only FILE.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

#define TCV_S2(x) #x
#define TCV_S(x) TCV_S2(x)
#define FN "tc_chr_neg_fgetc_write_only_errno_a" TCV_S(TC_CRT_VER) ".tmp"
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
    FILE *f;
    f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    errno = 0;
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK_EQ_I(errno, EBADF);
    fclose(f);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(write_only); DeleteFileA(FN); return tc_end(); }
