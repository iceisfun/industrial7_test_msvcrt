/* TC
 * id:    fio_neg_ungetc_writeonly
 * area:  stdio_file
 * kind:  negative
 * funcs: ungetc fopen
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   MS docs ungetc: returns EOF on failure; stream not opened for reading
 * note:  C11 does not define ungetc on a write-only stream; classic CRT rejects it (stream lacks _IOREAD) so EOF and no output is expected. Only return value and file emptiness are asserted (errno not asserted).
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
#define F "tc_fio_neg_ungetc_writeonly.tmp"

TC_CASE(w_mode)
{
    char b[8]; FILE *f; del(F);
    f = fopen(F, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ungetc('a', f), EOF);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(slurp(F, b, 8), 0);
    del(F);
}
int main(void) { tc_begin(); TC_RUN(w_mode); return tc_end(); }
