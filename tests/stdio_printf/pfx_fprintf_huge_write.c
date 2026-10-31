/* TC
 * id:    pfx_fprintf_huge_write
 * area:  stdio_printf
 * funcs: fprintf fopen fclose ftell
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1; MS docs fprintf (returns number of bytes/characters written)
 * note:  C only guarantees 4095 characters per call; that fprintf honours much larger outputs/widths unclipped is observed Windows CRT behaviour, not a standard guarantee.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define F "tc_pfx_fprintf_huge_write_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

static char *mk(size_t n)
{
    char *s = (char *)malloc(n + 1); size_t i;
    if (!s) return NULL;
    for (i = 0; i < n; i++) s[i] = (char)('A' + i % 26);
    s[n] = 0; return s;
}
static void one(size_t n)
{
    char *s = mk(n); FILE *f; int r; long pos, sz;
    CHECK(s != NULL); if (!s) return;
    f = fopen(F, "wb"); CHECK(f != NULL);
    if (f) {
        r = fprintf(f, "%s", s);
        CHECK_EQ_I(r, (long long)n);
        pos = ftell(f);
        CHECK_EQ_I(pos, (long long)n);
        CHECK_EQ_I(fclose(f), 0);
        sz = tc_slurp(F);
        CHECK_EQ_I(sz, (long long)n);
        if (sz == (long)n) {
            CHECK_MEM(tc_buf, s, 64);
            CHECK_MEM(tc_buf + n - 64, s + n - 64, 64);
            CHECK(memcmp(tc_buf, s, n) == 0);
        }
    }
    free(s);
}
TC_CASE(one_mib) { one(1048576); }
TC_CASE(boundary_4097) { one(4097); }
TC_CASE(boundary_8191) { one(8191); }
TC_CASE(boundaries_4095_4096_8192) { one(4095); one(4096); one(8192); }
TC_CASE(text_newlines)
{
    char *s = (char *)malloc(100001); FILE *f; long sz; int r;
    CHECK(s != NULL); if (!s) return;
    memset(s, '\n', 100000); s[100000] = 0;
    f = fopen(F, "w"); CHECK(f != NULL);
    if (f) {
        r = fprintf(f, "%s", s);
        CHECK_EQ_I(r, 100000);
        CHECK_EQ_I(fclose(f), 0);
        sz = tc_slurp(F);
        CHECK_EQ_I(sz, 200000);
        if (sz == 200000) { CHECK_MEM(tc_buf, "\r\n\r\n", 4); CHECK_MEM(tc_buf + 199996, "\r\n\r\n", 4); }
    }
    free(s);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(one_mib); TC_RUN(boundary_4097); TC_RUN(boundary_8191); TC_RUN(boundaries_4095_4096_8192); TC_RUN(text_newlines);
    remove(F); return tc_end();
}
