/* TC
 * id:    fio_neg_fread_writeonly
 * area:  stdio_file
 * kind:  negative
 * funcs: fread ferror fopen fclose clearerr
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS _filbuf behaviour (C11 7.21.8.1 does not define reading an output-only stream)
 * note:  Read from a write-only stream returns 0 and sets the error indicator. errno is tested in fio_neg_probe_fread_writeonly_errno.
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

static const char * volatile fname = "tc_fio_neg_fread_writeonly_a" TC_S(TC_CRT_VER) ".tmp";

TC_CASE(read_fails)
{
    unsigned char buf[8]; FILE *fp; size_t r; int i;
    for (i = 0; i < 8; i++) buf[i] = 0xAA;
    fp = fopen(fname, "w"); CHECK(fp != NULL); if (!fp) return;
    r = fread(buf, 1, 4, fp);
    CHECK_EQ_U(r, 0);
    CHECK(ferror(fp) != 0);
    for (i = 0; i < 8; i++) CHECK_EQ_I(buf[i], 0xAA);
    clearerr(fp);
    fclose(fp);
}
int main(void)
{
    tc_begin();
    TC_RUN(read_fails);
    DeleteFileA(fname);
    return tc_end();
}
