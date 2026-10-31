/* TC
 * id:    pfx_vfwprintf_basic
 * area:  stdio_printf
 * funcs: vfwprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.5 vfwprintf; C11 7.16.1.2 va_copy (MS docs vfwprintf)
 * note:  stream opened "w" (text, ANSI): wide output is converted to the multibyte code page (ASCII here). Binary-mode wide output writes raw 2-byte units, so "wb" is deliberately not used.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_vfwprintf_basic_a_" TC_CRT_DLL ".tmp"

static int wrap(FILE *f, const wchar_t *fmt, ...)
{
    int r; va_list ap; va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap); return r;
}
static int wrap2(FILE *f, const wchar_t *fmt, ...)   /* uses the same arguments twice via va_copy */
{
    int r1, r2; va_list ap, ap2; va_start(ap, fmt); va_copy(ap2, ap);
    r1 = vfwprintf(f, fmt, ap); r2 = vfwprintf(f, fmt, ap2);
    va_end(ap2); va_end(ap); return r1 * 100 + r2;
}
TC_CASE(ascii_to_ansi_file)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = wrap(f, L"hello %d", 42);
    CHECK_EQ_I(r, 8);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 8); CHECK_MEM(b, "hello 42", 8);
    remove(FN);
}
TC_CASE(int_and_wstr)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = wrap(f, L"[%d:%ls]", -7, L"abc");
    CHECK_EQ_I(r, 8);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 8); CHECK_MEM(b, "[-7:abc]", 8);
    remove(FN);
}
TC_CASE(va_copy_reuse)
{
    unsigned char b[64]; FILE *f = fopen(FN, "w"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = wrap2(f, L"<%d,%ls>", 5, L"z");
    CHECK_EQ_I(r, 505);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(n, 10); CHECK_MEM(b, "<5,z><5,z>", 10);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(ascii_to_ansi_file); TC_RUN(int_and_wstr); TC_RUN(va_copy_reuse); return tc_end(); }
