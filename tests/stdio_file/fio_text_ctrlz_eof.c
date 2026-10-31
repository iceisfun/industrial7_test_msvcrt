/* TC
 * id:    fio_text_ctrlz_eof
 * area:  stdio_file
 * funcs: fopen fgetc fread feof fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen / _read: Ctrl+Z is end-of-file in text mode
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
#define F "tc_fio_text_ctrlz_eof_" TC_CRT_DLL ".tmp"


TC_CASE(fread_stops)
{
    char b[16]; FILE *f; put(F, "abc\x1a" "def", 7); memset(b, 'Z', sizeof b);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 3); CHECK_MEM(b, "abc", 3); CHECK(feof(f) != 0); fclose(f); }
    del(F);
}
TC_CASE(fgetc_stops)
{
    FILE *f; put(F, "abc\x1a" "def", 7);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { int i; for (i = 0; i < 3; i++) CHECK_EQ_I(fgetc(f), 'a' + i); CHECK_EQ_I(fgetc(f), EOF); CHECK(feof(f) != 0); fclose(f); }
    del(F);
}
TC_CASE(rb_all)
{
    char b[16]; FILE *f; put(F, "abc\x1a" "def", 7);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 7); CHECK_MEM(b, "abc\x1a" "def", 7); fclose(f); }
    del(F);
}
TC_CASE(first_byte)
{
    char b[16]; FILE *f; put(F, "\x1a" "xyz", 4);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 0); CHECK(feof(f) != 0); fclose(f); }
    del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(fread_stops);
    TC_RUN(fgetc_stops);
    TC_RUN(rb_all);
    TC_RUN(first_byte);
    return tc_end();
}
