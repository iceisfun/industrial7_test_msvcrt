/* TC
 * id:    pfx_fprintf_text_cr_lf_literal
 * area:  stdio_printf
 * funcs: fprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fopen / _setmode: only LF is expanded to CR-LF in text mode
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
#define FN "tc_pfx_fprintf_text_cr_lf_literal_" TCS(TC_CRT_VER) ".tmp"
TC_CASE(crlf_literal)
{
    unsigned char b[8]; DWORD n;
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "\r\n"), 2);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 3);
    CHECK_MEM(b, "\r\r\n", 3);
    remove(FN);
}
TC_CASE(lone_cr)
{
    unsigned char b[8]; DWORD n;
    FILE *f = fopen(FN, "w"); CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "a\rb"), 3);
    fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_EQ_U(n, 3);
    CHECK_MEM(b, "a\rb", 3);
    remove(FN);
}
int main(void)
{
    tc_begin();
    TC_RUN(crlf_literal);
    TC_RUN(lone_cr);
    return tc_end();
}
