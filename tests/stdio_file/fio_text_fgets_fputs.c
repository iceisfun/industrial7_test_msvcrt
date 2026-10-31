/* TC
 * id:    fio_text_fgets_fputs
 * area:  stdio_file
 * funcs: fgets fputs fopen fclose
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   C11 7.21.7.2, 7.21.7.4; MS docs fopen text mode
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
#define F "tc_fio_text_fgets_fputs_" TC_CRT_DLL ".tmp"


TC_CASE(fputs_crlf)
{
    char b[16]; FILE *f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { CHECK(fputs("l1\nl2\n", f) >= 0); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 16), 8); CHECK_MEM(b, "l1\r\nl2\r\n", 8); del(F);
}
TC_CASE(fgets_lf)
{
    char l[16]; FILE *f; put(F, "l1\r\nl2\r\n", 8);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK(fgets(l, 16, f) != NULL); CHECK_STR(l, "l1\n"); CHECK(fgets(l, 16, f) != NULL); CHECK_STR(l, "l2\n"); CHECK(fgets(l, 16, f) == NULL); fclose(f); }
    del(F);
}
TC_CASE(last_no_newline)
{
    char l[16]; FILE *f; put(F, "l1\r\nlast", 8);
    f = fopen(F, "rt"); CHECK(f != NULL);
    if (f) { CHECK(fgets(l, 16, f) != NULL); CHECK(fgets(l, 16, f) != NULL); CHECK_STR(l, "last"); fclose(f); }
    del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(fputs_crlf);
    TC_RUN(fgets_lf);
    TC_RUN(last_no_newline);
    return tc_end();
}
