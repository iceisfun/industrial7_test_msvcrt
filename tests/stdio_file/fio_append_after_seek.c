/* TC
 * id:    fio_append_after_seek
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fseek fwrite fclose ftell
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3 (append: writes at end of file regardless of position)
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
#define F "tc_fio_append_after_seek.tmp"

TC_CASE(append_end)
{
    char b[16]; FILE *f; put(F, "abc", 3);
    f = fopen(F, "ab"); CHECK(f != NULL); if (!f) { del(F); return; }
    CHECK_EQ_I(fseek(f, 0, SEEK_SET), 0);
    CHECK_EQ_U(fwrite("X", 1, 1, f), 1);
    CHECK_EQ_I(fflush(f), 0);
    CHECK_EQ_I(ftell(f), 4);
    CHECK_EQ_I(fseek(f, 1, SEEK_SET), 0);
    CHECK_EQ_U(fwrite("Y", 1, 1, f), 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(slurp(F, b, 16), 5); CHECK_MEM(b, "abcXY", 5);
    del(F);
}
int main(void) { tc_begin(); TC_RUN(append_end); return tc_end(); }
