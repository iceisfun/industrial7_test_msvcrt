/* TC
 * id:    fio_neg_probe_ungetc_twice
 * area:  stdio_file
 * kind:  negative
 * funcs: ungetc fgetc fopen
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.7.10 only one pushback guaranteed
 * note:  PROBE: second consecutive ungetc without intervening read. Expected EOF (MS docs: 'only one character of pushback is guaranteed'); if a CRT accepts it, the data read back must still be consistent (checked loosely).
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
#define F "tc_fio_neg_probe_ungetc_twice.tmp"

TC_CASE(twice)
{
    FILE *f; int r2, a, b; put(F, "xyz", 3);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fgetc(f), 'x');
    CHECK_EQ_I(ungetc('A', f), 'A');
    r2 = ungetc('B', f);
    CHECK_EQ_I(r2, EOF);
    a = fgetc(f); b = fgetc(f);
    if (r2 == EOF) { CHECK_EQ_I(a, 'A'); CHECK_EQ_I(b, 'y'); }
    fclose(f); del(F);
}
int main(void) { tc_begin(); TC_RUN(twice); return tc_end(); }
