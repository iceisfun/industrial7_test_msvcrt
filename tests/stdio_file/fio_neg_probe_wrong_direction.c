/* TC
 * id:    fio_neg_probe_wrong_direction
 * area:  stdio_file
 * kind:  negative
 * funcs: fopen fclose fread fwrite ferror
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.21.5.3 (read from write-only / write to read-only stream is not supported); MS docs fread/fwrite
 * note:  Probes the exact failure signature of I/O in the wrong direction: count 0, ferror set (cases fread_on_write_only/fwrite_on_read_only), and separately errno EBADF (cases *_errno); also that the file is left unchanged.
 *        TRIAGE win10, class A): case fread_errno: errno stays 0 after fread on a write-only stream on ucrtbase 10.0.19045 (observed got=0);
 *        case fwrite_errno still sees EBADF. EBADF kept for non-UCRT in fread_errno (unverified).
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
#define F "tc_fio_neg_probe_wrong_direction_" TC_S1(TC_CRT_VER) ".tmp"

TC_CASE(fread_on_write_only)
{
    char b[4]; FILE *f = fopen(F, "w"); CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fread(b, 1, 4, f), 0); CHECK(ferror(f) != 0);
        fclose(f);
    }
    del(F);
}
TC_CASE(fwrite_on_read_only)
{
    FILE *f; char b[8]; put(F, "abc", 3);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) {
        CHECK_EQ_U(fwrite("zzzz", 1, 4, f), 0); CHECK(ferror(f) != 0);
        fclose(f);
    }
    CHECK_EQ_I(slurp(F, b, 8), 3); CHECK_MEM(b, "abc", 3);
    del(F);
}
TC_CASE(fread_errno)
{
    char b[4]; FILE *f = fopen(F, "w"); CHECK(f != NULL);
    if (f) { errno = 0; fread(b, 1, 4, f);
#if TC_AT_LEAST(140)
        CHECK_EQ_I(errno, 0);              /* observed ucrtbase 10.0.19045: errno untouched by fread on a write-only stream */
#else
        CHECK_EQ_I(errno, EBADF);
#endif
        fclose(f); }
    del(F);
}
TC_CASE(fwrite_errno)
{
    FILE *f; put(F, "abc", 3);
    f = fopen(F, "r"); CHECK(f != NULL);
    if (f) { errno = 0; fwrite("zzzz", 1, 4, f); CHECK_EQ_I(errno, EBADF); fclose(f); }
    del(F);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fread_on_write_only); TC_RUN(fwrite_on_read_only); TC_RUN(fread_errno); TC_RUN(fwrite_errno); return tc_end(); }
