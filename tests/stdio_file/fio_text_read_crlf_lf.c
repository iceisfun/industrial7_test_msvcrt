/* TC
 * id:    fio_text_read_crlf_lf
 * area:  stdio_file
 * funcs: fopen fread fgetc fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: CR-LF read as LF)
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
#define F "tc_fio_text_read_crlf_lf_" TC_CRT_DLL ".tmp"


TC_CASE(fgetc_rt)
{
    FILE *f; put(F, "a\r\nb", 4);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) {
        CHECK_EQ_I(fgetc(f), 'a'); CHECK_EQ_I(fgetc(f), '\n'); CHECK_EQ_I(fgetc(f), 'b'); CHECK_EQ_I(fgetc(f), EOF);
        fclose(f);
    }
    del(F);
}
TC_CASE(fread_rt)
{
    char b[16]; FILE *f; put(F, "a\r\nb\r\n", 6); memset(b, 'Z', sizeof b);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 4); CHECK_MEM(b, "a\nb\n", 4); fclose(f); }
    del(F);
}
TC_CASE(fread_5_of_raw)
{
    char b[16]; FILE *f; put(F, "a\r\nb\r\n", 6);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 5, f), 4); CHECK_MEM(b, "a\nb\n", 4); fclose(f); }
    del(F);
}
TC_CASE(rb_raw)
{
    char b[16]; FILE *f; put(F, "a\r\nb\r\n", 6);
    f = fopen(F, "rb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fread(b, 1, 16, f), 6); CHECK_MEM(b, "a\r\nb\r\n", 6); fclose(f); }
    del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(fgetc_rt);
    TC_RUN(fread_rt);
    TC_RUN(fread_5_of_raw);
    TC_RUN(rb_raw);
    return tc_end();
}
