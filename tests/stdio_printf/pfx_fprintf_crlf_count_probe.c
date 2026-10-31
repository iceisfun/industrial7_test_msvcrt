/* TC
 * id:    pfx_fprintf_crlf_count_probe
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  probe
 * ref:   MS docs fprintf (return value: number of bytes written)
 * note:  PROBE: for a text-mode stream, fprintf("a\n") returns the count of characters produced by the format (2), not
 *        the 3 bytes after CR LF translation, while the file holds 3 bytes. The file size is documented text-mode behaviour
 *        but is kept here together with the probed count only to tie them to the same call.
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

#define FN "tc_pfx_fprintf_crlf_count_probe_a_" TC_CRT_DLL ".tmp"

TC_CASE(count_excludes_cr)
{
    unsigned char b[16]; FILE *f = fopen(FN, "w"); int r, n;
    CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "a\n");
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_I(r, 2);
    CHECK_EQ_I(n, 3);
    remove(FN);
}
int main(void) { tc_begin(); remove(FN); TC_RUN(count_excludes_cr); return tc_end(); }
