/* TC
 * id:    pfx_fprintf_huge_width
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1 (field width); MS docs printf width specification
 * note:  C only guarantees 4095 characters per call; that fprintf honours much larger outputs/widths unclipped is observed Windows CRT behaviour, not a standard guarantee.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <string.h>
#define F "tc_pfx_fprintf_huge_width_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

static int allc(const unsigned char *p, size_t n, int c) { size_t i; for (i = 0; i < n; i++) if (p[i] != (unsigned char)c) return 0; return 1; }

TC_CASE(width_100000_d)
{
    FILE *f = fopen(F, "wb"); int r; long sz;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%100000d", 7);
    CHECK_EQ_I(r, 100000);
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 100000);
    if (sz == 100000) { CHECK(allc(tc_buf, 99999, ' ')); CHECK_EQ_I(tc_buf[99999], '7'); }
}
TC_CASE(left_justified_s_70000)
{
    FILE *f = fopen(F, "wb"); int r; long sz;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%-70000s", "xy");
    CHECK_EQ_I(r, 70000);
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 70000);
    if (sz == 70000) { CHECK_EQ_I(tc_buf[0], 'x'); CHECK_EQ_I(tc_buf[1], 'y'); CHECK(allc(tc_buf + 2, 69998, ' ')); }
}
TC_CASE(star_width_65536)
{
    FILE *f = fopen(F, "wb"); int r; long sz; volatile int w = 65536;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%*d", (int)w, 5);
    CHECK_EQ_I(r, 65536);
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 65536);
    if (sz == 65536) { CHECK(allc(tc_buf, 65535, ' ')); CHECK_EQ_I(tc_buf[65535], '5'); }
}
TC_CASE(zero_flag_width)
{
    FILE *f = fopen(F, "wb"); int r; long sz;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%080000d", -3);
    CHECK_EQ_I(r, 80000);
    CHECK_EQ_I(fclose(f), 0);
    sz = tc_slurp(F);
    CHECK_EQ_I(sz, 80000);
    if (sz == 80000) { CHECK_EQ_I(tc_buf[0], '-'); CHECK(allc(tc_buf + 1, 79998, '0')); CHECK_EQ_I(tc_buf[79999], '3'); }
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(width_100000_d); TC_RUN(left_justified_s_70000); TC_RUN(star_width_65536); TC_RUN(zero_flag_width);
    remove(F); return tc_end();
}
