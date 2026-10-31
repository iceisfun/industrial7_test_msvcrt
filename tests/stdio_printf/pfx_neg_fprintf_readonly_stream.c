/* TC
 * id:    pfx_neg_fprintf_readonly_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose ferror
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fprintf (negative value on error), ferror; C11 7.21.6.1
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#define F "tc_pfx_neg_fprintf_readonly_stream_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(write_to_read_stream_fails)
{
    FILE *w = fopen(F, "wb"), *r; int n;
    CHECK(w != NULL); if (!w) return;
    fputs("data", w); fclose(w);
    r = fopen(F, "rb");
    CHECK(r != NULL); if (!r) return;
    n = fprintf(r, "x%d", 1);
    CHECK(n < 0);
    CHECK(ferror(r) != 0);
    fclose(r);
    CHECK_EQ_I(tc_slurp(F), 4);     /* file untouched */
    CHECK_MEM(tc_buf, "data", 4);
}
int main(void)
{
    tc_begin(); remove(F);
    if (!tc_iph_install()) { /* none needed for old CRTs */ }
    TC_RUN(write_to_read_stream_fails);
    remove(F); return tc_end();
}
