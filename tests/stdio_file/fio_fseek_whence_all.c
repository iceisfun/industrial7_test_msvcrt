/* TC
 * id:    fio_fseek_whence_all
 * area:  stdio_file
 * funcs: fseek ftell fopen fgetc fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.21.9.2, 7.21.9.4; MS docs fseek (SEEK_END honoured on binary streams; C leaves that implementation-defined)
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

static const char * volatile fname = "tc_fio_fseek_whence_all_a" TC_S(TC_CRT_VER) ".tmp";
static volatile long off4 = 4, cur2 = 2, curm3 = -3, endm1 = -1;

TC_CASE(seek_set)
{
    FILE *fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fseek(fp, off4, SEEK_SET), 0);
    CHECK_EQ_I(ftell(fp), 4);
    CHECK_EQ_I(fgetc(fp), '4');
    CHECK_EQ_I(ftell(fp), 5);
    fclose(fp);
}
TC_CASE(seek_cur)
{
    FILE *fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fseek(fp, off4, SEEK_SET), 0);
    CHECK_EQ_I(fseek(fp, cur2, SEEK_CUR), 0);
    CHECK_EQ_I(ftell(fp), 6);
    CHECK_EQ_I(fgetc(fp), '6');
    CHECK_EQ_I(fseek(fp, curm3, SEEK_CUR), 0);
    CHECK_EQ_I(ftell(fp), 4);
    CHECK_EQ_I(fgetc(fp), '4');
    fclose(fp);
}
TC_CASE(seek_end)
{
    FILE *fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_I(fseek(fp, 0, SEEK_END), 0);
    CHECK_EQ_I(ftell(fp), 10);
    CHECK_EQ_I(fgetc(fp), EOF);
    CHECK_EQ_I(fseek(fp, endm1, SEEK_END), 0);
    CHECK_EQ_I(ftell(fp), 9);
    CHECK_EQ_I(fgetc(fp), '9');
    fclose(fp);
}
int main(void)
{
    tc_begin();
    mkfile(fname, "0123456789", 10);
    TC_RUN(seek_set); TC_RUN(seek_cur); TC_RUN(seek_end);
    DeleteFileA(fname);
    return tc_end();
}
