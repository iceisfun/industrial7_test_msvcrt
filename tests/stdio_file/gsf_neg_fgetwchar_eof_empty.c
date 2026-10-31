/* TC
 * id:    gsf_neg_fgetwchar_eof_empty
 * area:  stdio_file
 * funcs: _fgetwchar freopen feof
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.29.3.6 (getwchar returns WEOF at end of file); MS docs _fgetwchar (wint_t is 16-bit on Windows)
 * note:  Child process with stdin redirected to a file.
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
#define IN "tc_gsf_neg_fgetwchar_eof_empty_in_" SFX ".tmp"
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    if (!freopen(IN, "r", stdin)) return 3;
    if (is(c, "empty")) {
        wint_t r = _fgetwchar();
        if (r != WEOF) return 10;
        if (r != 0xFFFF) return 11;
        if (!feof(stdin)) return 12;
        return 0;
    }
    if (is(c, "data")) {
        if (_fgetwchar() != L'a') return 10;
        if (_fgetwchar() != WEOF) return 11;
        if (!feof(stdin)) return 12;
        return 0;
    }
    return 99;
}
static void run(const char *name, const char *data, DWORD n)
{
    DWORD code = 99;
    mk(IN, data, n);
    CHECK(tc_spawn_self(name, &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(IN);
}
TC_CASE(empty_file) { run("empty", "", 0); }
TC_CASE(after_data_then_eof) { run("data", "a", 1); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(empty_file); TC_RUN(after_data_then_eof);
    return tc_end();
}
