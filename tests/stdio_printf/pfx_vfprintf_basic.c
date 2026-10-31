/* TC
 * id:    pfx_vfprintf_basic
 * area:  stdio_printf
 * funcs: vfprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.6.8, 7.16.1.2 (va_copy); MS docs vfprintf
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <stdarg.h>
#define F "tc_pfx_vfprintf_basic_" TCV ".tmp"

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

static int twice(FILE *f, const char *fmt, ...)
{
    va_list ap, ap2; int a, b;
    va_start(ap, fmt); va_copy(ap2, ap);
    a = vfprintf(f, fmt, ap); b = vfprintf(f, fmt, ap2);
    va_end(ap2); va_end(ap);
    return (a == b) ? a : -999;
}
TC_CASE(wrapper_writes)
{
    FILE *f = fopen(F, "wb"); int n;
    CHECK(f != NULL); if (!f) return;
    n = wr(f, "%d-%s", 12, "ab");
    CHECK_EQ_I(n, 5);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 5);
    CHECK_MEM(tc_buf, "12-ab", 5);
}
TC_CASE(va_copy_twice)
{
    FILE *f = fopen(F, "wb"); int n;
    CHECK(f != NULL); if (!f) return;
    n = twice(f, "%d-%s;", 7, "xyz");
    CHECK_EQ_I(n, 6);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 12);
    CHECK_MEM(tc_buf, "7-xyz;7-xyz;", 12);
}
TC_CASE(text_mode_newline)
{
    FILE *f = fopen(F, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    n = wr(f, "%s\n%s\n", "p", "q");
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 6);
    CHECK_MEM(tc_buf, "p\r\nq\r\n", 6);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(wrapper_writes); TC_RUN(va_copy_twice); TC_RUN(text_mode_newline);
    remove(F); return tc_end();
}
