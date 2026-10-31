/* TC
 * id:    gsf_getws_s_basic
 * area:  stdio_file
 * funcs: _getws_s freopen
 * crt:   >=90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs gets_s, _getws_s (returns buffer on success, NULL on error or end-of-file)
 * note:  Child process with stdin redirected to an ASCII file; stdin is in ANSI text mode, so conversion to wide is trivial.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#define S2(x) #x
#define S1(x) S2(x)
#define SFX S1(TC_CRT_VER)
static void mk(const char *n, const void *d, DWORD l)
{
    HANDLE h = CreateFileA(n, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w = 0;
    if (h == INVALID_HANDLE_VALUE) return;
    if (l) WriteFile(h, d, l, &w, NULL);
    CloseHandle(h);
}
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static int isw(const wchar_t *a, const wchar_t *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
#define INF "tc_gsf_getws_s_basic_in_" SFX ".tmp"

static int child(const char *c)
{
    wchar_t buf[32]; wchar_t *volatile r;
    if (!freopen(INF, "r", stdin)) return 3;
    if (is(c, "lines")) {
        r = _getws_s(buf, 32); if (r != buf) return 10; if (!isw(buf, L"hello")) return 11;
        r = _getws_s(buf, 32); if (r != buf) return 12; if (!isw(buf, L"world")) return 13;
        r = _getws_s(buf, 32); if (r != NULL) return 14;
        return 0;
    }
    if (is(c, "exact")) {
        wchar_t b6[6];   /* "hello" + NUL */
        r = _getws_s(b6, 6); if (r != b6) return 10; if (!isw(b6, L"hello")) return 11;
        return 0;
    }
    if (is(c, "empty")) {
        buf[0] = L'z';
        r = _getws_s(buf, 32); if (r != buf) return 10; if (buf[0] != 0) return 11;
        return 0;
    }
    if (is(c, "eof")) {
        r = _getws_s(buf, 32); if (r != NULL) return 10;
        return 0;
    }
    return 99;
}
static void run(const char *name, const char *data, DWORD n)
{
    DWORD code = 99;
    mk(INF, data, n);
    CHECK(tc_spawn_self(name, &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(INF);
}
TC_CASE(ascii_line) { run("lines", "hello\nworld\n", 12); }
TC_CASE(exact_fit_buffer) { run("exact", "hello\n", 6); }
TC_CASE(empty_line) { run("empty", "\n", 1); }
TC_CASE(eof_returns_null) { run("eof", "", 0); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(ascii_line); TC_RUN(exact_fit_buffer); TC_RUN(empty_line); TC_RUN(eof_returns_null);
    return tc_end();
}
