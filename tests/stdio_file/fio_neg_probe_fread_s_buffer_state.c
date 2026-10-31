/* TC
 * id:    fio_neg_probe_fread_s_buffer_state
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose
 * procs: fread_s
 * crt:   >=80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fread_s (buffer contents on failure)
 * note:  PROBE: after fread_s fails because bufferSize is too small, the buffer is entirely zeroed (docs hint at this).
 *        The return value 0 and errno are separate cases.
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

typedef size_t (__cdecl *fread_s_t)(void *, size_t, size_t, size_t, FILE *);
static fread_s_t p_fread_s;
static const char * volatile fname = "tc_fio_neg_probe_fread_s_buffer_state_a" TC_S(TC_CRT_VER) ".tmp";
static volatile size_t cnt = 32;

TC_CASE(buffer_zeroed)
{
    unsigned char buf[16]; FILE *fp; int i, nz = 0;
    if (!p_fread_s) TC_SKIP("fread_s not exported");
    for (i = 0; i < 16; i++) buf[i] = 0xAA;
    fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    p_fread_s(buf, 16, 1, cnt, fp);
    for (i = 0; i < 16; i++) if (buf[i] != 0) nz++;
    CHECK_EQ_I(nz, 0);
    fclose(fp);
}
TC_CASE(fail_return)
{
    unsigned char buf[16]; FILE *fp; size_t r;
    if (!p_fread_s) TC_SKIP("fread_s not exported");
    fp = fopen(fname, "rb"); CHECK(fp != NULL); if (!fp) return;
    errno = 0;
    r = p_fread_s(buf, 16, 1, cnt, fp);
    CHECK_EQ_U(r, 0);
    CHECK_EQ_I(errno, ERANGE);
    fclose(fp);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_fread_s = (fread_s_t)tc_crt_proc("fread_s");
    mkfile(fname, "0123456789abcdef0123456789abcdef0123456789", 42);
    TC_RUN(buffer_zeroed); TC_RUN(fail_return);
    DeleteFileA(fname);
    return tc_end();
}
