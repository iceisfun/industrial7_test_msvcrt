/* TC
 * id:    pfx_probe_fprintf_huge_precision
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   C11 7.21.6.1 (precision of integer conversions: minimum digits)
 * note:  PROBE: very large integer precision (%.50000d) is honoured by every CRT generation (zero padded to 50000 digits), not clipped to an internal buffer size.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#define F "tc_pfx_probe_fprintf_huge_precision_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(prec_50000)
{
    FILE *f = fopen(F, "wb"); int r; long sz; size_t i, bad = 0;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%.50000d", 9);
    CHECK_EQ_I(r, 50000);
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 50000);
    if (sz == 50000) { for (i = 0; i < 49999; i++) if (tc_buf[i] != '0') bad++; CHECK_EQ_I(bad, 0); CHECK_EQ_I(tc_buf[49999], '9'); }
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(prec_50000);
    remove(F); return tc_end();
}
