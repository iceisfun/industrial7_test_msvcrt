/* TC
 * id:    fio_neg_fseek_bad_origin
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose fseek ftell
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fseek: invalid origin or negative resulting position -> nonzero return, EINVAL
 * note:  Invalid-parameter handler count is deliberately not asserted (handler may or may not run); handler is installed only so a fatal default cannot kill the test.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)   /* file size via kernel32, -1 if missing */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static int exists(const char *n) { return GetFileAttributesA(n) != INVALID_FILE_ATTRIBUTES; }
static void del(const char *n) { DeleteFileA(n); }
#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define F "tc_fio_neg_fseek_bad_origin_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(bad_origin)
{
    FILE *f; volatile int org = 7; int r; put(F, "abcde", 5);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) {
        tc_iph_count = 0; errno = 0;
        r = fseek(f, 0, org); CHECK(r != 0); CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(ftell(f), 0);
        fclose(f);
    }
    del(F);
}
TC_CASE(before_start)
{
    FILE *f; int r; put(F, "abcde", 5);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) {
        tc_iph_count = 0; errno = 0;
        r = fseek(f, -1, SEEK_SET); CHECK(r != 0); CHECK_EQ_I(errno, EINVAL);
        CHECK_EQ_I(ftell(f), 0);
        fclose(f);
    }
    del(F);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(bad_origin); TC_RUN(before_start); return tc_end(); }
