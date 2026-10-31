/* TC
 * id:    fio_freopen_basic
 * area:  stdio_file
 * funcs: freopen fopen fclose fread fwrite
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.4 (freopen); MS docs freopen
 */
#include "tc.h"
#include <stdio.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)

static void mk(const char *n, const char *d)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0, l = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    while (d[l]) l++;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, char *b, int cap)  /* returns bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap - 1, &r, NULL)) r = 0;
    CloseHandle(h); b[r] = 0; return (int)r;
}

#define A "tc_fio_freopen_basic_a_" SFX ".tmp"
#define B "tc_fio_freopen_basic_b_" SFX ".tmp"
#define C "tc_fio_freopen_basic_c_" SFX ".tmp"

TC_CASE(switch_files)
{
    FILE *fp, *r; char buf[32]; size_t n; char out[64];
    mk(A, "AAAA"); mk(B, "BBBBBB"); DeleteFileA(C);
    fp = fopen(A, "r"); CHECK(fp != NULL); if (!fp) return;
    r = freopen(B, "r", fp);
    CHECK(r == fp);
    if (r) {
        n = fread(buf, 1, sizeof buf - 1, r); buf[n] = 0;
        CHECK_EQ_U(n, 6); CHECK_STR(buf, "BBBBBB");
        r = freopen(C, "w", r);
        CHECK(r == fp);
        if (r) { CHECK_EQ_U(fwrite("xyz", 1, 3, r), 3); fclose(r); }
    }
    CHECK_EQ_I(rd(C, out, sizeof out), 3); CHECK_STR(out, "xyz");
    CHECK_EQ_I(rd(A, out, sizeof out), 4); CHECK_STR(out, "AAAA");
    CHECK_EQ_I(rd(B, out, sizeof out), 6); CHECK_STR(out, "BBBBBB");
    DeleteFileA(A); DeleteFileA(B); DeleteFileA(C);
}
int main(void) { tc_begin(); TC_RUN(switch_files); return tc_end(); }
