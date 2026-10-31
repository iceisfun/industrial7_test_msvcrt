/* TC
 * id:    gsf_fgetwchar_fputwchar_std
 * area:  stdio_file
 * funcs: _fgetwchar _fputwchar freopen fflush
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fgetwchar, _fputwchar (equivalent to getwchar / putwchar); freopen ccs= (VS2005+)
 * note:  ANSI-mode redirected streams, ASCII range only. The ccs=UTF-16LE case needs msvcr80+ and is skipped on msvcrt.dll.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
static void mk(const char *n, const void *d, DWORD l)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    WriteFile(h, d, l, &w, NULL); CloseHandle(h);
}
static int rd(const char *n, unsigned char *b, int cap)  /* bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap, &r, NULL)) r = 0;
    CloseHandle(h); return (int)r;
}
#define IN  "tc_gsf_fgetwchar_fputwchar_std_in_" SFX ".tmp"
#define OUT "tc_gsf_fgetwchar_fputwchar_std_out_" SFX ".tmp"
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    if (is(c, "get")) {
        if (!freopen(IN, "r", stdin)) return 3;
        if (_fgetwchar() != L'a') return 10;
        if (_fgetwchar() != L'b') return 11;
        if (_fgetwchar() != WEOF) return 12;
        return 0;
    }
    if (is(c, "putz")) {
        if (!freopen(OUT, "w", stdout)) return 3;
        if (_fputwchar(L'Z') != L'Z') return 10;
        if (fflush(stdout) != 0) return 11;
        return 0;
    }
    if (is(c, "uni")) {
        volatile wchar_t wc = 0x0416;
        if (!freopen(OUT, "w,ccs=UTF-16LE", stdout)) return 3;
        if (_fputwchar(wc) != 0x0416) return 10;
        if (fflush(stdout) != 0) return 11;
        return 0;
    }
    return 99;
}
TC_CASE(fgetwchar_ascii)
{
    DWORD code = 99;
    mk(IN, "ab", 2);
    CHECK(tc_spawn_self("get", &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(IN);
}
TC_CASE(fputwchar_ascii)
{
    DWORD code = 99; unsigned char b[8];
    DeleteFileA(OUT);
    CHECK(tc_spawn_self("putz", &code, 20000)); CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(OUT, b, sizeof b), 1);
    CHECK_EQ_U(b[0], 'Z');
    DeleteFileA(OUT);
}
TC_CASE(unicode_mode_roundtrip)
{
    DWORD code = 99; unsigned char b[8];
    if (!TC_AT_LEAST(80)) TC_SKIP("ccs= needs msvcr80+");
    DeleteFileA(OUT);
    CHECK(tc_spawn_self("uni", &code, 20000)); CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(OUT, b, sizeof b), 4);
    CHECK_EQ_U(b[0], 0xFF); CHECK_EQ_U(b[1], 0xFE); CHECK_EQ_U(b[2], 0x16); CHECK_EQ_U(b[3], 0x04);
    DeleteFileA(OUT);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(fgetwchar_ascii); TC_RUN(fputwchar_ascii); TC_RUN(unicode_mode_roundtrip);
    return tc_end();
}
