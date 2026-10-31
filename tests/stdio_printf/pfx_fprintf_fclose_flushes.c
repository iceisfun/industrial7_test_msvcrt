/* TC
 * id:    pfx_fprintf_fclose_flushes
 * area:  stdio_printf
 * funcs: fprintf fopen fclose setvbuf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.1 (fclose flushes unwritten buffered data); MS docs fclose
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#define F "tc_pfx_fprintf_fclose_flushes_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(binary_size_and_content)
{
    FILE *f = fopen(F, "wb"); int n, r; long sz;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IOFBF, 4096), 0);
    n = fprintf(f, "hello %d", 42);
    CHECK_EQ_I(n, 8);
    r = fclose(f);
    CHECK_EQ_I(r, 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 8);
    CHECK_MEM(tc_buf, "hello 42", 8);
}
TC_CASE(reopen_reads_back)
{
    FILE *f = fopen(F, "rb"); char b[16] = {0}; size_t got;
    CHECK(f != NULL); if (!f) return;
    got = fread(b, 1, sizeof b - 1, f);
    CHECK_EQ_U(got, 8);
    CHECK_STR(b, "hello 42");
    CHECK_EQ_I(fclose(f), 0);
}
TC_CASE(text_mode_size)
{
    FILE *f = fopen(F, "w"); int n; long sz;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IOFBF, 4096), 0);
    n = fprintf(f, "a\nb\n");
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 6);
    CHECK_MEM(tc_buf, "a\r\nb\r\n", 6);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(binary_size_and_content); TC_RUN(reopen_reads_back); TC_RUN(text_mode_size);
    remove(F); return tc_end();
}
