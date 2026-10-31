/* TC
 * id:    fio_append_two_handles
 * area:  stdio_file
 * kind:  positive
 * funcs: fopen fwrite fflush fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   C11 7.21.5.3; each append write goes to the current end of file
 * note:  Two independent FILE handles opened with mode a on the same file; every flushed write must land at the current EOF.
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
#define F "tc_fio_append_two_handles.tmp"

TC_CASE(two)
{
    char b[16]; FILE *h1, *h2; del(F);
    h1 = fopen(F, "ab"); h2 = fopen(F, "ab"); CHECK(h1 != NULL); CHECK(h2 != NULL);
    if (h1 && h2) {
        CHECK_EQ_U(fwrite("AA", 1, 2, h1), 2); CHECK_EQ_I(fflush(h1), 0);
        CHECK_EQ_U(fwrite("BB", 1, 2, h2), 2); CHECK_EQ_I(fflush(h2), 0);
        CHECK_EQ_U(fwrite("CC", 1, 2, h1), 2); CHECK_EQ_I(fflush(h1), 0);
    }
    if (h1) fclose(h1);
    if (h2) fclose(h2);
    CHECK_EQ_I(slurp(F, b, 16), 6); CHECK_MEM(b, "AABBCC", 6);
    del(F);
}
int main(void) { tc_begin(); TC_RUN(two); return tc_end(); }
