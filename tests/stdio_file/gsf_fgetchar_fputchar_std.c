/* TC
 * id:    gsf_fgetchar_fputchar_std
 * area:  stdio_file
 * funcs: _fgetchar _fputchar freopen fflush
 * crt:   all
 * tier:  p0
 * kind:  positive
 * conf:  documented
 * ref:   MS docs _fgetchar, _fputchar (equivalent to getchar / putchar)
 * note:  No console I/O: stdin/stdout are redirected to files with freopen inside a child process; the parent checks
 *        the child's exit code and the output file via kernel32.
 */
#include "tc.h"
#include <stdio.h>
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
#define IN  "tc_gsf_fgetchar_fputchar_std_in_" SFX ".tmp"
#define OUT "tc_gsf_fgetchar_fputchar_std_out_" SFX ".tmp"
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    if (is(c, "seq")) {
        if (!freopen(IN, "r", stdin)) return 3;
        if (_fgetchar() != 'A') return 10;
        if (_fgetchar() != 'B') return 11;
        if (_fgetchar() != '\n') return 12;
        if (_fgetchar() != 'C') return 13;
        if (_fgetchar() != EOF) return 14;
        return 0;
    }
    if (is(c, "high")) {
        if (!freopen(IN, "rb", stdin)) return 3;
        if (_fgetchar() != 255) return 10;
        if (_fgetchar() != EOF) return 11;
        return 0;
    }
    if (is(c, "putx")) {
        if (!freopen(OUT, "wb", stdout)) return 3;
        if (_fputchar('x') != 'x') return 10;
        if (fflush(stdout) != 0) return 11;
        return 0;
    }
    if (is(c, "pute9")) {
        volatile int ch = 0xE9;
        if (!freopen(OUT, "wb", stdout)) return 3;
        if (_fputchar(ch) != 0xE9) return 10;
        if (fflush(stdout) != 0) return 11;
        return 0;
    }
    return 99;
}
TC_CASE(fgetchar_sequence)
{
    DWORD code = 99;
    mk(IN, "AB\nC", 4);
    CHECK(tc_spawn_self("seq", &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(IN);
}
TC_CASE(fputchar_writes_stdout)
{
    DWORD code = 99; unsigned char b[8];
    DeleteFileA(OUT);
    CHECK(tc_spawn_self("putx", &code, 20000)); CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(OUT, b, sizeof b), 1);
    CHECK_EQ_U(b[0], 'x');
    DeleteFileA(OUT);
}
TC_CASE(fputchar_return_unsigned)
{
    DWORD code = 99; unsigned char b[8];
    DeleteFileA(OUT);
    CHECK(tc_spawn_self("pute9", &code, 20000)); CHECK_EQ_U(code, 0);
    CHECK_EQ_I(rd(OUT, b, sizeof b), 1);
    CHECK_EQ_U(b[0], 0xE9);
    DeleteFileA(OUT);
}
TC_CASE(fgetchar_high_byte_unsigned)
{
    DWORD code = 99; static const unsigned char d[1] = { 0xFF };
    mk(IN, d, 1);
    CHECK(tc_spawn_self("high", &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(IN);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(fgetchar_sequence); TC_RUN(fputchar_writes_stdout); TC_RUN(fputchar_return_unsigned); TC_RUN(fgetchar_high_byte_unsigned);
    return tc_end();
}
