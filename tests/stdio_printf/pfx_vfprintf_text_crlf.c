/* TC
 * id:    pfx_vfprintf_text_crlf
 * area:  stdio_printf
 * funcs: vfprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs vfprintf / fopen (text mode LF -> CRLF; return value counts characters, not translated CRs)
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <stdarg.h>
#define F "tc_pfx_vfprintf_text_crlf_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

static int wr(FILE *f, const char *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = vfprintf(f, fmt, ap); va_end(ap); return r;
}

TC_CASE(text_count_excludes_cr)
{
    FILE *f = fopen(F, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    n = wr(f, "\n\n%d\n", 5);
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 7);
    CHECK_MEM(tc_buf, "\r\n\r\n5\r\n", 7);
}
TC_CASE(binary_exact)
{
    FILE *f = fopen(F, "wb"); int n;
    CHECK(f != NULL); if (!f) return;
    n = wr(f, "\n\n%d\n", 5);
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 4);
    CHECK_MEM(tc_buf, "\n\n5\n", 4);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(text_count_excludes_cr); TC_RUN(binary_exact);
    remove(F); return tc_end();
}
