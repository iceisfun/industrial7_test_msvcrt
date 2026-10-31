/* TC
 * id:    fio_text_write_lf_crlf
 * area:  stdio_file
 * funcs: fopen fwrite fputc fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: LF written as CR-LF)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

static int slurp(const char *n, char *b, int cap)
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, (DWORD)cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
static void del(const char *n) { DeleteFileA(n); }
#define F "tc_fio_text_write_lf_crlf_" TC_CRT_DLL ".tmp"


TC_CASE(wt_fwrite)
{
    char b[32]; FILE *f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("a\nb\n", 1, 4, f), 4); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 32), 6); CHECK_MEM(b, "a\r\nb\r\n", 6); del(F);
}
TC_CASE(fputc_nl)
{
    char b[32]; FILE *f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_I(fputc('\n', f), '\n'); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 32), 2); CHECK_MEM(b, "\r\n", 2); del(F);
}
TC_CASE(existing_crlf)
{
    char b[32]; FILE *f = fopen(F, "wt"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("x\r\ny", 1, 4, f), 4); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 32), 5); CHECK_MEM(b, "x\r\r\ny", 5); del(F);
}
TC_CASE(wb_untouched)
{
    char b[32]; FILE *f = fopen(F, "wb"); CHECK(f != NULL);
    if (f) { CHECK_EQ_U(fwrite("a\nb\n", 1, 4, f), 4); CHECK_EQ_I(fclose(f), 0); }
    CHECK_EQ_I(slurp(F, b, 32), 4); CHECK_MEM(b, "a\nb\n", 4); del(F);
}
int main(void)
{
    tc_begin();
    TC_RUN(wt_fwrite);
    TC_RUN(fputc_nl);
    TC_RUN(existing_crlf);
    TC_RUN(wb_untouched);
    return tc_end();
}
