/* TC
 * id:    pfx_fwprintf_ansi_file
 * area:  stdio_printf
 * funcs: fwprintf fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  documented
 * ref:   MS docs fwprintf, fopen (no ccs= flag: ANSI mode; wide output converted to multibyte), printf type field (%ls, %hs)
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <wchar.h>
#define F "tc_pfx_fwprintf_ansi_file_" TCV ".tmp"

static unsigned char tc_buf[1 << 21];
/* read a whole file through kernel32 (an external handle, independent of the CRT); -1 if cannot open */
static long tc_slurp(const char *p)
{
    DWORD n = 0; HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, 0, NULL);
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, tc_buf, sizeof tc_buf, &n, NULL)) n = 0;
    CloseHandle(h); return (long)n;
}

TC_CASE(ascii_narrowed)
{
    FILE *f = fopen(F, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"abc %d", 1);
    CHECK_EQ_I(n, 5);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 5);
    CHECK_MEM(tc_buf, "abc 1", 5);
}
TC_CASE(text_newline)
{
    FILE *f = fopen(F, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"\n");
    CHECK_EQ_I(n, 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 2);
    CHECK_MEM(tc_buf, "\r\n", 2);
}
TC_CASE(explicit_ls_hs)
{
    FILE *f = fopen(F, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"[%ls|%hs|%5ls]", L"wide", "narrow", L"ab");
    CHECK_EQ_I(n, 19);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 19);
    CHECK_MEM(tc_buf, "[wide|narrow|   ab]", 19);
}
TC_CASE(text_two_newlines)
{
    FILE *f = fopen(F, "w"); int n;
    CHECK(f != NULL); if (!f) return;
    n = fwprintf(f, L"a\nb\n");
    CHECK_EQ_I(n, 4);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(tc_slurp(F), 6);
    CHECK_MEM(tc_buf, "a\r\nb\r\n", 6);
}
int main(void)
{
    tc_begin(); remove(F);
    TC_RUN(ascii_narrowed); TC_RUN(text_newline); TC_RUN(explicit_ls_hs); TC_RUN(text_two_newlines);
    remove(F); return tc_end();
}
