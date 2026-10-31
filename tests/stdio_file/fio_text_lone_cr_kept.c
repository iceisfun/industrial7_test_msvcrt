/* TC
 * id:    fio_text_lone_cr_kept
 * area:  stdio_file
 * funcs: fopen fgetc fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs fopen; CR not followed by LF is not translated
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

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
#define F "tc_fio_text_lone_cr_kept_" TC_CRT_DLL ".tmp"


TC_CASE(mid_cr)
{
    FILE *f; put(F, "a\rb", 3);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), '\r'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), EOF); fclose(f); }
    del(F);
}
TC_CASE(trailing_cr)
{
    FILE *f; put(F, "a\r", 2);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), '\r'); CHECK_EQ_I(fgetc(f), EOF); fclose(f); }
    del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(mid_cr);
    TC_RUN(trailing_cr);
    return tc_end();
}
