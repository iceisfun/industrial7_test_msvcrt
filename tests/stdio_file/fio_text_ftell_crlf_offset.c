/* TC
 * id:    fio_text_ftell_crlf_offset
 * area:  stdio_file
 * funcs: fopen fgets ftell fclose
 * crt:   all
 * tier:  p1
 * conf:  observed
 * ref:   MS docs ftell (position is a byte offset; text-mode CR-LF counts as 2)
 * note:  PROBE-like: asserts ftell after reading "ab\r\n" in text mode equals the raw file offset 4, not the translated count 3.
 *        Not guaranteed by ISO C (text-mode ftell value is unspecified).
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
#define F "tc_fio_text_ftell_fseek_roundtrip_" TC_CRT_DLL ".tmp"


TC_CASE(after_line1)
{
    FILE *f; char l[16]; put(F, "ab\r\ncd", 6);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK(fgets(l, 16, f) != NULL); CHECK_STR(l, "ab\n"); CHECK_EQ_I(ftell(f), 4); fclose(f); }
    del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(after_line1);
    return tc_end();
}
