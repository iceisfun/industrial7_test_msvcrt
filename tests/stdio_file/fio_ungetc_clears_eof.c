/* TC
 * id:    fio_ungetc_clears_eof
 * area:  stdio_file
 * kind:  positive
 * funcs: ungetc feof fgetc fopen
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.7.10 (successful ungetc clears end-of-file indicator); MS docs ungetc
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
#define F "tc_fio_ungetc_clears_eof.tmp"

TC_CASE(clears)
{
    FILE *f; put(F, "ab", 2);
    f = fopen(F, "rb"); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    CHECK_EQ_I(ungetc('x', f), 'x');
    CHECK_EQ_I(feof(f), 0);
    CHECK_EQ_I(fgetc(f), 'x');
    CHECK_EQ_I(fgetc(f), EOF);
    CHECK(feof(f) != 0);
    fclose(f); del(F);
}
int main(void) { tc_begin(); TC_RUN(clears); return tc_end(); }
