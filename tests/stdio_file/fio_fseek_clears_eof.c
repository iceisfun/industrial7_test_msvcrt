/* TC
 * id:    fio_fseek_clears_eof
 * area:  stdio_file
 * funcs: fseek feof fread fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.9.2p5 (successful fseek clears the end-of-file indicator)
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

static const char * volatile fname = "tc_fio_fseek_clears_eof_a" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(clears)
{
    char b[32]; FILE *fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    CHECK_EQ_U(fread(b, 1, 20, fp), 10);
    CHECK(feof(fp) != 0);
    CHECK_EQ_I(fseek(fp, 0, SEEK_SET), 0);
    CHECK_EQ_I(feof(fp), 0);
    CHECK_EQ_U(fread(b, 1, 1, fp), 1);
    CHECK_EQ_I(b[0], '0');
    fclose(fp);
}
int main(void)
{
    tc_begin();
    mkfile(fname, "0123456789", 10);
    TC_RUN(clears);
    DeleteFileA(fname);
    return tc_end();
}
