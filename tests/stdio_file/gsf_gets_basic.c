/* TC
 * id:    gsf_gets_basic
 * area:  stdio_file
 * funcs:
 * procs: gets freopen fclose
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90 msvcr80 msvcrt
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs gets (reads a line from stdin, replaces the newline with NUL; removed from ucrtbase, use gets_s)
 * note:  Child process with stdin redirected to a file; gets is resolved at run time.
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
#define IN "tc_gsf_gets_basic_in_" SFX ".tmp"
typedef char *(__cdecl *gets_t)(char *);
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }

static int child(const char *c)
{
    gets_t g = (gets_t)tc_crt_proc("gets"); char buf[64];
    if (!g) return 2;
    if (!freopen(IN, "r", stdin)) return 3;
    if (is(c, "lines")) {
        if (g(buf) != buf) return 10;
        if (!is(buf, "hello")) return 11;
        if (g(buf) != buf) return 12;
        if (!is(buf, "world")) return 13;
        if (g(buf) != NULL) return 14;
        return 0;
    }
    if (is(c, "empty")) {
        buf[0] = 'z';
        if (g(buf) != buf) return 10;
        if (buf[0] != 0) return 11;
        return 0;
    }
    if (is(c, "tail")) {
        if (g(buf) != buf) return 10;
        if (!is(buf, "tail")) return 11;
        if (g(buf) != NULL) return 12;
        return 0;
    }
    if (is(c, "eof")) {
        if (g(buf) != NULL) return 10;
        return 0;
    }
    if (is(c, "crlf")) {
        if (g(buf) != buf) return 10;
        if (!is(buf, "ab")) return 11;
        if (g(buf) != buf) return 12;
        if (!is(buf, "cd")) return 13;
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
TC_CASE(line_strip_newline) { run("lines", "hello\nworld\n", 12); }
TC_CASE(empty_line) { run("empty", "\n", 1); }
TC_CASE(last_line_no_newline) { run("tail", "tail", 4); }
TC_CASE(eof_returns_null) { run("eof", "", 0); }
TC_CASE(crlf_text_mode) { run("crlf", "ab\r\ncd\r\n", 8); }
TC_CASE(exported)
{
    if (!tc_crt_proc("gets")) TC_SKIP("gets not exported");
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    if (!tc_crt_proc("gets")) { TC_RUN(exported); return tc_end(); }
    TC_RUN(line_strip_newline); TC_RUN(empty_line); TC_RUN(last_line_no_newline); TC_RUN(eof_returns_null); TC_RUN(crlf_text_mode);
    return tc_end();
}
