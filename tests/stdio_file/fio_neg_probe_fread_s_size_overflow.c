/* TC
 * id:    fio_neg_probe_fread_s_size_overflow
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: fread_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fread_s (bufferSize < elementSize*count -> EINVAL/ERANGE style failure, returns 0)
 * note:  PROBE: fread_s(buf,16,SIZE_MAX/2+1,2,fp) where elementSize*count wraps to 0. Probed: returns 0 without crashing
 *        and errno is ERANGE or EINVAL (UCRT checks count > bufferSize/elementSize, so overflow is caught).
 */
#include "tc.h"
#define TC_S2(x) #x
#define TC_S(x) TC_S2(x)
#include <stdio.h>
#include <errno.h>
#include <stdint.h>

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

typedef size_t (__cdecl *fread_s_t)(void *, size_t, size_t, size_t, FILE *);
static fread_s_t p_fread_s;
static const char * volatile fname = "tc_fio_neg_probe_fread_s_size_overflow_a" TC_S(TC_CRT_VER) ".tmp";
static volatile size_t big = (size_t)-1 / 2 + 1;

TC_CASE(returns_zero)
{
    unsigned char buf[16]; FILE *fp; size_t r; int i;
    if (!p_fread_s) TC_SKIP("fread_s not exported");
    for (i = 0; i < 16; i++) buf[i] = 0xAA;
    fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    r = p_fread_s(buf, 16, big, 2, fp);
    CHECK_EQ_U(r, 0);
    fclose(fp);
}
TC_CASE(errno_set)
{
    unsigned char buf[16]; FILE *fp; int e;
    if (!p_fread_s) TC_SKIP("fread_s not exported");
    fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    errno = 0;
    p_fread_s(buf, 16, big, 2, fp);
    e = errno;
    CHECK(e == ERANGE || e == EINVAL);
    fclose(fp);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_fread_s = (fread_s_t)tc_crt_proc("fread_s");
    mkfile(fname, "0123456789abcdef0123", 20);
    TC_RUN(returns_zero); TC_RUN(errno_set);
    DeleteFileA(fname);
    return tc_end();
}
