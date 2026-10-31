/* TC
 * id:    pfm_2_stdio_common_wide_stream_conv_matrix
 * area:  stdio_printf
 * funcs: __stdio_common_vswprintf __stdio_common_vfwprintf __stdio_common_vfprintf
 * crt:   >=140
 * tier:  p0
 * conf:  observed
 * ref:   Windows SDK corecrt_stdio_config.h / stdio.h / wchar.h (declared, otherwise undocumented); C11 7.21.6.1 for the conversions
 * note:  Direct calls with options 0 and locale NULL (what the inline wrappers pass). Streams are ANSI text-mode temp files
 *        with ASCII content, read back through kernel32.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define FNW "tc_pfm_2_scwsc_w.tmp"
#define FNA "tc_pfm_2_scwsc_a.tmp"

static int sw(wchar_t *b, size_t n, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vswprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int fw(FILE *s, const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfwprintf(0, s, f, NULL, ap); va_end(ap); return r; }
static int fa(FILE *s, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vfprintf(0, s, f, NULL, ap); va_end(ap); return r; }

static int slurp(const char *name, char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max - 1, &n, NULL)) n = 0;
    CloseHandle(h); buf[n] = 0; return (int)n;
}
static void note(const char *k, long long v) { char b[32]; tc_write("TC NOTE " TC_TEST_ID " "); tc_write(k); tc_write("="); tc_i64(v, b); tc_write(b); tc_write("\n"); }
static volatile int vm5 = -5, v7 = 7, v8 = 8, v255 = 255, vc = 'c';
static volatile unsigned vbig = 4000000000u;
static volatile double v15 = 1.5, v1 = 1.0;

TC_CASE(sw_ints)  { wchar_t b[64]; int r = sw(b, 64, L"%d%i%u%o%x%X", vm5, v7, vbig, v8, v255, v255); CHECK_WSTR(b, L"-574000000000" L"10ffFF"); CHECK_EQ_I(r, 19); }
TC_CASE(sw_floats)
{
    wchar_t b[96]; int r = sw(b, 96, L"%.1e|%.1E|%.1f|%.1F|%g|%G", v15, v15, v15, v15, v15, v15);
    CHECK_WSTR(b, L"1.5e+00|1.5E+00|1.5|1.5|1.5|1.5"); CHECK_EQ_I(r, 31);
}
TC_CASE(sw_hexfloat_pct)
{
    wchar_t b[64]; int r = sw(b, 64, L"%.1a|%.1A|100%%", v1, v1);
    CHECK_WSTR(b, L"0x1.0p+0|0X1.0P+0|100%"); CHECK_EQ_I(r, 22);
}
/* truncation return value with options 0 is only recorded (not documented for the raw function) */
TC_CASE(sw_count_semantics)
{
    wchar_t b[8]; int r = sw(b, 4, L"%d", 123456);
    note("opt0_count4_ret", r);
    CHECK(b[0] == L'1' && b[1] == L'2' && b[2] == L'3');
}
TC_CASE(fw_file)
{
    FILE *f = fopen(FNW, "w"); char b[64]; int r, n;
    CHECK(f != NULL); if (!f) return;
    r = fw(f, L"%d|%x|%ls|%.2f", vm5, v255, L"w", v15); fclose(f);
    n = slurp(FNW, b, sizeof b);
    CHECK_STR(b, "-5|ff|w|1.50"); CHECK_EQ_I(r, 12); CHECK_EQ_I(n, 12); remove(FNW);
}
TC_CASE(fa_file)
{
    FILE *f = fopen(FNA, "w"); char b[96]; int r, n;
    CHECK(f != NULL); if (!f) return;
    r = fa(f, "%c %s %p %%", vc, "str", (void *)(size_t)0x1234); fclose(f);
    n = slurp(FNA, b, sizeof b);
    CHECK_STR(b, TC_X64 ? "c str 0000000000001234 %" : "c str 00001234 %");
    CHECK_EQ_I(r, TC_X64 ? 24 : 16); CHECK_EQ_I(n, r); remove(FNA);
}
int main(void)
{
    tc_begin();
    TC_RUN(sw_ints); TC_RUN(sw_floats); TC_RUN(sw_hexfloat_pct); TC_RUN(sw_count_semantics); TC_RUN(fw_file); TC_RUN(fa_file);
    return tc_end();
}
