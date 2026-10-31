/* TC
 * id:    gsf_neg_getws_s_line_too_long
 * area:  stdio_file
 * funcs: _getws_s freopen
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs gets_s, _getws_s (line does not fit: buffer[0] = 0, ERANGE, invalid-parameter handler, returns NULL)
 * note:  Child process with stdin redirected; the buffer size includes room for the terminating NUL. Child reports through its exit code.
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
#define INF "tc_gsf_neg_getws_s_line_too_long_in_" SFX ".tmp"

static int child(const char *c)
{
    wchar_t buf[4]; wchar_t *volatile r; int e; volatile size_t n4 = 4;
    if (!freopen(INF, "r", stdin)) return 3;
    tc_iph_install();
    buf[0] = L'Q'; buf[1] = L'Q'; buf[2] = L'Q'; buf[3] = L'Q';
    errno = 0;
    r = _getws_s(buf, n4); e = errno;
    if (is(c, "ok")) {
        if (r != buf) return 20;
        if (!isw(buf, L"abc")) return 21;
        if (tc_iph_count != 0) return 22;
        return 0;
    }
    if (r != NULL) return 10;
    if (buf[0] != 0) return 11;
    if (e != ERANGE) return 12;
    if (tc_iph_count != 1) return 13;
    return 0;
}
static void run(const char *name, const char *data, DWORD n)
{
    DWORD code = 99;
    mk(INF, data, n);
    CHECK(tc_spawn_self(name, &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(INF);
}
TC_CASE(too_long) { run("fail", "abcdefgh\n", 9); }
TC_CASE(exact_len_no_room) { run("fail", "abcd\n", 5); }
TC_CASE(one_less_ok) { run("ok", "abc\n", 4); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(too_long); TC_RUN(exact_len_no_room); TC_RUN(one_less_ok);
    return tc_end();
}
