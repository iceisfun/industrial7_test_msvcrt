/* TC
 * id:    pfx_probe_fprintf_text_ctrl_z_write
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs fopen (Ctrl+Z is an EOF marker on READ in text mode)
 * note:  Probes: writing 0x1A via %c to a text-mode "w" stream is stored verbatim (no truncation, no translation), size 3.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static DWORD slurp(const char *n, unsigned char *b, DWORD cap)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return (DWORD)-1;
    if (!ReadFile(h, b, cap, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return got;
}

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_probe_fprintf_text_ctrl_z_write_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(ctrl_z_written_verbatim)
{
    unsigned char b[8]; DWORD n; volatile int z = 0x1A;
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a%cb", z), 3);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 3);
    CHECK_MEM(b, "a\x1a" "b", 3);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(ctrl_z_written_verbatim);
    return tc_end();
}
