/* TC
 * id:    gsf_vscanf_basic
 * area:  stdio_file
 * funcs: freopen
 * procs: vscanf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C99 7.19.6.12 / C11 7.21.6.12 vscanf; MS docs vscanf (msvcr120 is the first CRT exporting it; ucrtbase via __stdio_common_vfscanf)
 * note:  Child process with stdin redirected to a file; vscanf is reached through a va_list wrapper.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
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
static int rd(const char *n, char *b, int cap)  /* bytes read, -1 if cannot open */
{
    HANDLE h = CreateFileA(n, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL); DWORD r = 0;
    if (h == INVALID_HANDLE_VALUE) return -1;
    if (!ReadFile(h, b, cap - 1, &r, NULL)) r = 0;
    CloseHandle(h); b[r] = 0; return (int)r;
}
static int is(const char *a, const char *b) { while (*a && *a == *b) { a++; b++; } return *a == *b; }
static int vsc(const char *f, ...)
{
    va_list ap; int r;
#if TC_CRT_VER >= 140
    va_start(ap, f); r = vscanf(f, ap); va_end(ap);   /* inline in ucrt headers -> __stdio_common_vfscanf */
#else
    typedef int (__cdecl *vscanf_t)(const char *, va_list);
    vscanf_t p = (vscanf_t)tc_crt_proc("vscanf");
    if (!p) return -99;
    va_start(ap, f); r = p(f, ap); va_end(ap);
#endif
    return r;
}
#define INF "tc_gsf_vscanf_basic_in_" SFX ".tmp"

static int child(const char *c)
{
    int a = -1, b = -1, n = -1, r; char s[16]; char ch = 0; unsigned x = 0;
    if (!freopen(INF, "r", stdin)) return 3;
    if (is(c, "intstr")) {
        r = vsc("%d %7s", &a, s); if (r != 2) return 10; if (a != 42) return 11; if (!is(s, "hello")) return 12;
        return 0;
    }
    if (is(c, "hexchar")) {
        r = vsc("%x %c", &x, &ch); if (r != 2) return 10; if (x != 255) return 11; if (ch != 'Z') return 12;
        return 0;
    }
    if (is(c, "nsupp")) {
        r = vsc("%*d %d%n", &a, &n); if (r != 1) return 10; if (a != 20) return 11; if (n != 5) return 12;
        return 0;
    }
    if (is(c, "partial")) {
        r = vsc("%s %d", s, &a); if (r != 1) return 10; if (!is(s, "ab")) return 11; if (a != -1) return 12;
        return 0;
    }
    if (is(c, "seq")) {
        r = vsc("%d", &a); if (r != 1 || a != 1) return 10;
        r = vsc("%d", &a); if (r != 1 || a != 2) return 11;
        r = vsc("%d", &a); if (r != 1 || a != 3) return 12;
        r = vsc("%d", &a); if (r != EOF) return 13;
        return 0;
    }
    (void)b;
    return 99;
}
static void run(const char *name, const char *data, DWORD n)
{
    DWORD code = 99;
    mk(INF, data, n);
    CHECK(tc_spawn_self(name, &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(INF);
}
TC_CASE(int_and_string) { run("intstr", "42 hello\n", 9); }
TC_CASE(hex_char_conversions) { run("hexchar", "ff Z\n", 5); }
TC_CASE(percent_n_and_suppression) { run("nsupp", "10 20\n", 6); }
TC_CASE(returns_count_partial) { run("partial", "ab", 2); }
TC_CASE(uses_stdin_position) { run("seq", "1 2\n3\n", 6); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(int_and_string); TC_RUN(hex_char_conversions); TC_RUN(percent_n_and_suppression);
    TC_RUN(returns_count_partial); TC_RUN(uses_stdin_position);
    return tc_end();
}
