/* TC
 * id:    pfw_vfwprintf_utf16le_file
 * area:  stdio_printf
 * funcs: vfwprintf _wfopen
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs vfwprintf, fopen ccs=UTF-16LE
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define TC_S2(x) #x
#define TC_S1(x) TC_S2(x)
#define FN "tc_pfw_vf16_" TC_S1(TC_CRT_VER) ".tmp"
#define WFN L"" FN
static int slurp(unsigned char *buf, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, cap, &n, NULL)) n = (DWORD)-1;
    CloseHandle(h); return (int)n;
}
static int vf(FILE *f, const wchar_t *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap); return r;
}
static int u16eq(const unsigned char *b, const char *ascii)  /* b points past the BOM */
{
    int i; for (i = 0; ascii[i]; i++) if (b[2 * i] != (unsigned char)ascii[i] || b[2 * i + 1] != 0) return 0;
    return 1;
}
TC_CASE(basic)
{
    unsigned char b[128]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n;
    CHECK(f != NULL); if (!f) return;
    vf(f, L"x=%d", 42); fclose(f);
    n = slurp(b, 128); CHECK_EQ_I(n, 2 + 8); CHECK(b[0] == 0xFF && b[1] == 0xFE); CHECK(u16eq(b + 2, "x=42"));
    DeleteFileA(FN);
}
TC_CASE(return_count)
{
    FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n;
    CHECK(f != NULL); if (!f) return;
    n = vf(f, L"%ls-%d", L"abc", 99); CHECK_EQ_I(n, 6);
    fclose(f); DeleteFileA(FN);
}
TC_CASE(width_and_precision)
{
    unsigned char b[128]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n, r;
    CHECK(f != NULL); if (!f) return;
    r = vf(f, L"[%6.2ls][%-4d]", L"abcdef", 7); fclose(f);
    CHECK_EQ_I(r, 14);
    n = slurp(b, 128); CHECK_EQ_I(n, 2 + 28); CHECK(u16eq(b + 2, "[    ab][7   ]"));
    DeleteFileA(FN);
}
TC_CASE(multiple_calls_append)
{
    unsigned char b[128]; FILE *f = _wfopen(WFN, L"w, ccs=UTF-16LE"); int n;
    CHECK(f != NULL); if (!f) return;
    vf(f, L"ab"); vf(f, L"%c", (int)L'c'); vf(f, L"%d", 12); fclose(f);
    n = slurp(b, 128); CHECK_EQ_I(n, 2 + 10); CHECK(u16eq(b + 2, "abc12"));
    DeleteFileA(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(basic); TC_RUN(return_count); TC_RUN(width_and_precision); TC_RUN(multiple_calls_append);
    return tc_end();
}
