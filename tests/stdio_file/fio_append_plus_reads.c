/* TC
 * id:    fio_append_plus_reads
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fread fwrite rewind fclose fseek
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.3 (a+: read anywhere, write at end)
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
#define F "tc_fio_append_plus_reads.tmp"

TC_CASE(aplus)
{
    char b[16]; FILE *f; put(F, "abc", 3);
    f = fopen(F, "a+b"); CHECK(f != NULL); if (!f) { del(F); return; }
    rewind(f);
    CHECK_EQ_U(fread(b, 1, 3, f), 3); CHECK_MEM(b, "abc", 3);
    CHECK_EQ_I(fseek(f, 0, SEEK_END), 0);
    CHECK_EQ_U(fwrite("d", 1, 1, f), 1);
    rewind(f);
    CHECK_EQ_U(fread(b, 1, 8, f), 4); CHECK_MEM(b, "abcd", 4);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(slurp(F, b, 16), 4); CHECK_MEM(b, "abcd", 4);
    del(F);
}
int main(void) { tc_begin(); TC_RUN(aplus); return tc_end(); }
