/* TC
 * id:    fio_fseek_beyond_eof
 * area:  stdio_file
 * funcs: fseek ftell fgetc fputc fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   Win32 SetFilePointer allows seeking past EOF; extending write zero-fills the gap (NTFS)
 * note:  Seek past end of a binary file succeeds on the MS CRT; the gap reads as zero bytes after a write.
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <stdio.h>

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

static const char * volatile fname = "tc_fio_fseek_beyond_eof_a" TC_S(TC_CRT_VER) ".tmp";
static volatile long off100 = 100;

TC_CASE(seek_and_read)
{
    FILE *fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fseek(fp, off100, SEEK_SET), 0);
    CHECK_EQ_I(ftell(fp), 100);
    CHECK_EQ_I(fgetc(fp), EOF);
    fclose(fp);
}
TC_CASE(write_extends)
{
    FILE *fp = fopen(fname, "r+b"); HANDLE h; DWORD got = 0; unsigned char buf[101]; int i, nz = 0;
    CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fseek(fp, off100, SEEK_SET), 0);
    CHECK_EQ_I(fputc('Z', fp), 'Z');
    CHECK_EQ_I(fclose(fp), 0);
    CHECK_EQ_I(fsize(fname), 101);
    h = CreateFileA(fname, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    CHECK(h != INVALID_HANDLE_VALUE); if (h == INVALID_HANDLE_VALUE) return;
    ReadFile(h, buf, 101, &got, NULL); CloseHandle(h);
    CHECK_EQ_U(got, 101);
    CHECK_MEM(buf, "0123456789", 10);
    for (i = 10; i < 100; i++) if (buf[i] != 0) nz++;
    CHECK_EQ_I(nz, 0);
    CHECK_EQ_I(buf[100], 'Z');
}
int main(void)
{
    tc_begin();
    mkfile(fname, "0123456789", 10);
    TC_RUN(seek_and_read); TC_RUN(write_extends);
    DeleteFileA(fname);
    return tc_end();
}
