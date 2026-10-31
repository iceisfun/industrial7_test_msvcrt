/* TC
 * id:    pfx_fprintf_null_string_prec_probe
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs "Format specification syntax"
 * note:  PROBE: precision applies to the substituted "(null)" text, so fprintf("%.3s", NULL) writes "(nu" (3 bytes).
 */
#include "tc.h"
#include <stdio.h>

/* read whole file via kernel32 (no CRT) */
static int slurp(const char *name, unsigned char *buf, int max)
{
    DWORD n = 0; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, buf, (DWORD)max, &n, NULL)) n = 0;
    CloseHandle(h); return (int)n;
}

#define FN "tc_pfx_fprintf_null_string_prec_probe_a_" TC_CRT_DLL ".tmp"

TC_CASE(prec3_null)
{
    unsigned char b[32]; FILE *f = fopen(FN, "w"); int r, n; const char *volatile p = NULL;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "%.3s", p);
    fclose(f);
    CHECK_EQ_I(r, 3);
    n = slurp(FN, b, sizeof b); CHECK_EQ_I(n, 3); CHECK_MEM(b, "(nu", 3);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(prec3_null); return tc_end(); }
