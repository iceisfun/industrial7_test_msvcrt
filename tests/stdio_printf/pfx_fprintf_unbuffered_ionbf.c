/* TC
 * id:    pfx_fprintf_unbuffered_ionbf
 * area:  stdio_printf
 * funcs: fprintf fopen setvbuf fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs setvbuf (_IONBF: no buffer is used); C11 7.21.5.6
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#define F "tc_pfx_fprintf_unbuffered_ionbf_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(setvbuf_returns_zero)
{
    FILE *f = fopen(F, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    CHECK_EQ_I(fclose(f), 0);
}
TC_CASE(visible_without_flush)
{
    FILE *f = fopen(F, "wb"); int n; long sz;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    n = fprintf(f, "abc%d", 123);
    CHECK_EQ_I(n, 6);
    sz = tc_slurp(F);       /* no fflush: written through */
    CHECK_EQ_I(sz, 6);
    CHECK_MEM(tc_buf, "abc123", 6);
    CHECK_EQ_I(fclose(f), 0);
}
TC_CASE(accumulate_in_order)
{
    FILE *f = fopen(F, "wb"); long sz;
    CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(setvbuf(f, NULL, _IONBF, 0), 0);
    CHECK_EQ_I(fprintf(f, "one;"), 4);
    sz = tc_slurp(F); CHECK_EQ_I(sz, 4);
    CHECK_EQ_I(fprintf(f, "%s;", "two"), 4);
    sz = tc_slurp(F); CHECK_EQ_I(sz, 8);
    CHECK_EQ_I(fprintf(f, "%d", 3), 1);
    sz = tc_slurp(F); CHECK_EQ_I(sz, 9);
    CHECK_MEM(tc_buf, "one;two;3", 9);
    CHECK_EQ_I(fclose(f), 0);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(setvbuf_returns_zero); TC_RUN(visible_without_flush); TC_RUN(accumulate_in_order);
    remove(F); return tc_end();
}
