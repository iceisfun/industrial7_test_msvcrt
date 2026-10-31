/* TC
 * id:    pfx_fprintf_mixed_ordering
 * area:  stdio_printf
 * funcs: fprintf fputs fputc fwrite fopen fclose setvbuf
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.3 (a stream is a single ordered sequence); MS docs fprintf, fputs, fputc, fwrite
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <string.h>
#define F "tc_pfx_fprintf_mixed_ordering_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

static void seq(FILE *f, int nl)
{
    static const char blk[] = "CC";
    volatile size_t zero = 0;
    fprintf(f, "A%d", 1);
    fputs(nl ? "B\n" : "B2", f);
    fwrite(blk, 1, 0 + 2, f);
    fwrite(blk, 1, zero, f);           /* no-op */
    fwrite(blk, zero, 2, f);           /* no-op */
    fputc(nl ? '\n' : 'D', f);
    fprintf(f, nl ? "E%s\n" : "E%s", "F");
}
TC_CASE(binary_order)
{
    FILE *f = fopen(F, "wb"); long sz;
    CHECK(f != NULL); if (!f) return;
    seq(f, 0); CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F); CHECK_EQ_I(sz, 9);
    CHECK_MEM(tc_buf, "A1B2CCDEF", 9);
}
TC_CASE(unbuffered_order)
{
    FILE *f = fopen(F, "wb"); long sz;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    seq(f, 0);
    sz = tc_slurp(F); CHECK_EQ_I(sz, 9);
    CHECK_MEM(tc_buf, "A1B2CCDEF", 9);
    CHECK_EQ_I(fclose(f), 0);
}
TC_CASE(text_mode_newlines)
{
    FILE *f = fopen(F, "w"); long sz;
    CHECK(f != NULL); if (!f) return;
    seq(f, 1); CHECK_EQ_I(fclose(f), 0);
    /* A1 B\n CC \n EF\n  -> "A1B\r\nCC\r\nEF\r\n" */
    sz = tc_slurp(F); CHECK_EQ_I(sz, 13);
    CHECK_MEM(tc_buf, "A1B\r\nCC\r\nEF\r\n", 13);
}
TC_CASE(zero_count_write_returns_zero)
{
    FILE *f = fopen(F, "wb"); volatile size_t z = 0; size_t a, b;
    CHECK(f != NULL); if (!f) return;
    fprintf(f, "x");
    a = fwrite("zz", 1, z, f); b = fwrite("zz", z, 3, f);
    CHECK_EQ_U(a, 0); CHECK_EQ_U(b, 0);
    fprintf(f, "y");
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 2);
    CHECK_MEM(tc_buf, "xy", 2);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(binary_order); TC_RUN(unbuffered_order); TC_RUN(text_mode_newlines); TC_RUN(zero_count_write_returns_zero);
    remove(F); return tc_end();
}
