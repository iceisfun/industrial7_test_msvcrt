/* TC
 * id:    fio_neg_fwrite_readonly
 * area:  stdio_file
 * kind:  negative
 * funcs: fwrite ferror fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.8.2; MS _flsbuf: stream not opened for writing -> EBADF
 * note:  fwrite to a stream opened "r" returns 0, sets the error indicator and errno EBADF; file contents unchanged.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <stdio.h>
#include <errno.h>

static void mkfile(const char *name, const char *data, unsigned n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    if (n) WriteFile(h, data, n, &w, NULL);
    CloseHandle(h);
}
static long long fsize(const char *name)
{
    WIN32_FILE_ATTRIBUTE_DATA d;
    if (!GetFileAttributesExA(name, GetFileExInfoStandard, &d)) return -1;
    return ((long long)d.nFileSizeHigh << 32) | d.nFileSizeLow;
}

static const char * volatile fname = "tc_fio_neg_fwrite_readonly_a" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(write_fails)
{
    FILE *fp; size_t r; errno = 0;
    fp = fopen(fname, "r"); CHECK(fp != NULL); if (!fp) return;
    r = fwrite("xyz", 1, 3, fp);
    CHECK_EQ_U(r, 0);
    CHECK(ferror(fp) != 0);
    CHECK_EQ_I(errno, EBADF);
    fclose(fp);
}
TC_CASE(file_unchanged)
{
    char b[8] = {0}; FILE *fp; size_t r;
    fp = fopen(fname, "r"); CHECK(fp != NULL); if (!fp) return;
    fwrite("xyz", 1, 3, fp);
    fclose(fp);
    CHECK_EQ_I(fsize(fname), 5);
    fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    r = fread(b, 1, 8, fp);
    CHECK_EQ_U(r, 5);
    CHECK_STR(b, "hello");
    fclose(fp);
}
int main(void)
{
    tc_begin();
    mkfile(fname, "hello", 5);
    TC_RUN(write_fails); TC_RUN(file_unchanged);
    DeleteFileA(fname);
    return tc_end();
}
