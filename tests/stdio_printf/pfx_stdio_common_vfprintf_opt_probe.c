/* TC
 * id:    pfx_stdio_common_vfprintf_opt_probe
 * area:  stdio_printf
 * funcs: __stdio_common_vfprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   Windows SDK corecrt_stdio_config.h (_CRT_INTERNAL_PRINTF_STANDARD_SNPRINTF_BEHAVIOR = 0x2)
 * note:  PROBE: the snprintf-behavior option bit only affects the buffer (vsprintf) path; on a stream the output and return
 *        value are identical to options 0.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_stdio_common_vfprintf_opt_probe_a_" TC_CRT_DLL ".tmp"

static int c_opt(unsigned __int64 o, FILE *f, const char *fmt, ...)
{ int r; va_list ap; va_start(ap, fmt); r = __stdio_common_vfprintf(o, f, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(snprintf_bit_no_effect)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; volatile int v = 12;
    CHECK(f != NULL); if (!f) return;
    r = c_opt(0x2ULL, f, "x=%d", (int)v); fclose(f);
    CHECK_EQ_I(r, 4);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 4); CHECK_MEM(b, "x=12", 4);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(snprintf_bit_no_effect); return tc_end(); }
