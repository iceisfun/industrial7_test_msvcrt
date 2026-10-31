/* TC
 * id:    fio_fwrite_basic
 * area:  stdio_file
 * funcs: fwrite fopen fclose ftell
 * procs: 
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.8.2; MS docs fwrite
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static void mkfile(const char *name, const void *data, DWORD n)
{
    DWORD w; HANDLE h = CreateFileA(name, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h == INVALID_HANDLE_VALUE) return;
    if (n) WriteFile(h, data, n, &w, NULL);
    CloseHandle(h);
}
static DWORD fsize(const char *name)
{
    DWORD s = 0xffffffffu; HANDLE h = CreateFileA(name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { s = GetFileSize(h, NULL); CloseHandle(h); }
    return s;
}

#define BIG 100000
static const char *volatile fn = "tc_fio_fwrite_basic_" TC_CRT_DLL ".tmp";
static size_t volatile v1 = 1, v3 = 3, v4 = 4, v5 = 5, vbig = BIG;
static char big[BIG];

TC_CASE(five)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("hello", 1, v5, f), 5); CHECK_EQ_I(ftell(f), 5); fclose(f);
    CHECK_EQ_U(fsize(fn), 5);
}
TC_CASE(elems)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite("abcdefghijkl", v4, v3, f), 3); CHECK_EQ_I(ftell(f), 12); fclose(f);
    CHECK_EQ_U(fsize(fn), 12);
}
TC_CASE(large)
{
    FILE *f; size_t i; for (i = 0; i < BIG; i++) big[i] = (char)(i * 7);
    f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_U(fwrite(big, v1, vbig, f), BIG); CHECK_EQ_I(ftell(f), BIG); fclose(f);
    CHECK_EQ_U(fsize(fn), BIG);
}
TC_CASE(tell_tracks)
{
    FILE *f = fopen(fn, "wb"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(ftell(f), 0);
    fwrite("ab", 1, v1 + 1, f); CHECK_EQ_I(ftell(f), 2);
    fwrite("cdef", 1, v4, f); CHECK_EQ_I(ftell(f), 6);
    fclose(f);
}
int main(void)
{
    tc_begin(); TC_RUN(five); TC_RUN(elems); TC_RUN(large); TC_RUN(tell_tracks);
    remove(fn); return tc_end();
}
