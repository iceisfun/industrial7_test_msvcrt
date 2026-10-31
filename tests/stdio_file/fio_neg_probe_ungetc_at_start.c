/* TC
 * id:    fio_neg_probe_ungetc_at_start
 * area:  stdio_file
 * kind:  negative
 * funcs: ungetc fgetc fopen
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   C11 7.21.7.10
 * note:  PROBE: ungetc before any read on a fresh 'rb' stream (no buffer allocated yet). Expected success (classic CRT: ptr==base with cnt==0 permits pushback); if EOF is returned, fgetc must return the first file byte.
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
#define F "tc_fio_neg_probe_ungetc_at_start.tmp"

TC_CASE(at_start)
{
    FILE *f; int r; put(F, "abc", 3);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) { del(F); return; }
    r = ungetc('Z', f);
    CHECK_EQ_I(r, 'Z');
    if (r == 'Z') CHECK_EQ_I(fgetc(f), 'Z');
    else CHECK_EQ_I(fgetc(f), 'a');
    fclose(f); del(F);
}
int main(void) { tc_begin(); TC_RUN(at_start); return tc_end(); }
