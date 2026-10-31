/* TC
 * id:    gsf_neg_vscanf_matching_input_failure
 * area:  stdio_file
 * funcs: freopen
 * procs: vscanf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.21.6.2p16 / 7.21.6.12 (EOF if input failure before first conversion; count of assigned items on matching failure)
 * note:  Child process with stdin redirected. A matching failure leaves the offending character unread.
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
#define INF "tc_gsf_neg_vscanf_matching_input_failure_in_" SFX ".tmp"

static int child(const char *c)
{
    int a = -1, b = -1, r; char s[16];
    if (!freopen(INF, "r", stdin)) return 3;
    if (is(c, "match")) {
        r = vsc("%d", &a); if (r != 0) return 10; if (a != -1) return 11;
        r = vsc("%7s", s); if (r != 1) return 12; if (!is(s, "abc")) return 13;
        return 0;
    }
    if (is(c, "empty")) {
        r = vsc("%d", &a); if (r != EOF) return 10; if (a != -1) return 11;
        return 0;
    }
    if (is(c, "partial")) {
        r = vsc("%d %d", &a, &b); if (r != 1) return 10; if (a != 5) return 11; if (b != -1) return 12;
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
TC_CASE(matching_failure) { run("match", "abc\n", 4); }
TC_CASE(input_failure_empty) { run("empty", "", 0); }
TC_CASE(partial_then_eof) { run("partial", "5", 1); }
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(matching_failure); TC_RUN(input_failure_empty); TC_RUN(partial_then_eof);
    return tc_end();
}
