/* TC
 * id:    pfx_fprintf_text_crlf_count
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen (text mode: LF translated to CR-LF on output); C11 7.21.2
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static DWORD slurp(const char *n, unsigned char *b, DWORD cap)
{
    DWORD got = 0; HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return (DWORD)-1;
    if (!ReadFile(h, b, cap, &got, NULL)) got = (DWORD)-1;
    CloseHandle(h); return got;
}

#define TCS2(x) #x
#define TCS(x) TCS2(x)
#define FN "tc_pfx_fprintf_text_crlf_count_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(two_lines)
{
    unsigned char b[32]; DWORD n; volatile int r;
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    r = fprintf(f, "a\nb\n");
    CHECK_EQ_I(r, 4);
    CHECK_EQ_I(fclose(f), 0);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 6);
    CHECK_MEM(b, "a\r\nb\r\n", 6);
    remove(FN);
}
TC_CASE(percent_s_newline)
{
    unsigned char b[32]; DWORD n; volatile const char *s = "x\ny";
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "<%s>", (const char *)s), 5);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 6);
    CHECK_MEM(b, "<x\r\ny>", 6);
    remove(FN);
}
TC_CASE(single_newline)
{
    unsigned char b[8]; DWORD n;
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "\n"), 1);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 2);
    CHECK_MEM(b, "\r\n", 2);
    remove(FN);
}

int main(void)
{
    tc_begin();
    TC_RUN(two_lines);
    TC_RUN(percent_s_newline);
    TC_RUN(single_newline);
    return tc_end();
}
