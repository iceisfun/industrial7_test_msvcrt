/* TC
 * id:    pfx_probe_fwprintf_binary_mode_wide
 * area:  stdio_printf
 * funcs: fwprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fputwc / fwprintf (stream orientation by open mode)
 * note:  PROBE: on a file opened "wb" (binary, no ccs) fwprintf does NOT narrow: each wchar_t is written as 2 raw bytes (UTF-16LE), no newline translation. Return value is the character count.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <wchar.h>
#define F "tc_pfx_probe_fwprintf_binary_mode_wide_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(binary_writes_wide)
{
    FILE *f = fopen(F, "wb"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"a\nb");
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 6);
    CHECK_MEM(tc_buf, "a\0\n\0b\0", 6);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(binary_writes_wide);
    remove(F); return tc_end();
}
