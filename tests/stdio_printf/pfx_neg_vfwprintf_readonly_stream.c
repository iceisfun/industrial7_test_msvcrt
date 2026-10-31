/* TC
 * id:    pfx_neg_vfwprintf_readonly_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: vfwprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.2.5 (vfwprintf returns a negative value if an output or encoding error occurred); MS docs vfwprintf
 * note:  Whether the ferror indicator or errno is set is not asserted. An invalid-parameter handler is installed where
 *        available only so that a CRT that treats the wrong-mode stream as invalid does not terminate.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

#define FN "tc_pfx_neg_vfwprintf_readonly_stream.tmp"

static int wv(FILE *f, const wchar_t *fmt, ...)
{
    va_list ap; int r;
    va_start(ap, fmt); r = vfwprintf(f, fmt, ap); va_end(ap);
    return r;
}
static void mkfile(void)
{
    DWORD n; HANDLE h = CreateFileA(FN, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    WriteFile(h, "hello", 5, &n, NULL); CloseHandle(h);
}
static int readback(char *b, int cap)
{
    DWORD n = 0; HANDLE h = CreateFileA(FN, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    ReadFile(h, b, cap - 1, &n, NULL); CloseHandle(h); b[n] = 0; return (int)n;
}

TC_CASE(fails)
{
    FILE *f; int r;
    mkfile();
    f = fopen(FN, "r"); CHECK(f != NULL); if (!f) return;
    r = wv(f, L"%d", 12345);
    CHECK(r < 0);
    fclose(f);
}
TC_CASE(unchanged)
{
    char b[32]; int n = readback(b, sizeof b);
    CHECK_EQ_I(n, 5);
    CHECK_STR(b, "hello");
    DeleteFileA(FN);
}
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(fails); TC_RUN(unchanged); return tc_end(); }
