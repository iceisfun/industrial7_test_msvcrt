/* TC
 * id:    fio_neg_fwrite_null_buffer
 * area:  stdio_file
 * kind:  negative
 * funcs: fwrite fopen fclose
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fwrite: NULL buffer invokes invalid parameter handler, errno EINVAL, returns 0
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

static const char * volatile fname = "tc_fio_neg_fwrite_null_buffer_a" TC_S(TC_CRT_VER) ".tmp";
static const void * volatile nul = NULL;

TC_CASE(null_buffer)
{
    FILE *fp; size_t r; int n0 = tc_iph_count; errno = 0;
    fp = fopen(fname, "wb"); CHECK(fp != NULL); if (!fp) return;
    r = fwrite(nul, 1, 4, fp);
    CHECK_EQ_U(r, 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count - n0, 1);
    CHECK_EQ_I(fclose(fp), 0);
    CHECK_EQ_I(fsize(fname), 0);
}
int main(void)
{
    tc_begin();
    CHECK(tc_iph_install());
    TC_RUN(null_buffer);
    DeleteFileA(fname);
    return tc_end();
}
