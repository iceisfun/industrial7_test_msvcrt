/* TC
 * id:    fio_neg_probe_ungetc_after_write_rplus
 * area:  stdio_file
 * kind:  negative
 * funcs: ungetc fputc fopen
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.5.3 (input after output requires intervening flush/seek)
 * note:  PROBE: r+ stream, fputc then ungetc without an intervening seek/flush. Expected EOF.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static void put(const char *n, const char *d, int len)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { if (len) WriteFile(h, d, (DWORD)len, &w, NULL); CloseHandle(h); }
}
static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_neg_probe_ungetc_after_write_rplus.tmp"

TC_CASE(after_write)
{
    FILE *f; put(F, "abc", 3);
    f = fopen(F, "r+b"); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fputc('Q', f), 'Q');
    CHECK_EQ_I(ungetc('Z', f), EOF);
    fclose(f); del(F);
}
int main(void) { tc_begin(); TC_RUN(after_write); return tc_end(); }
