/* TC
 * id:    gsf_neg_vscanf_default_handler_fatal
 * area:  stdio_file
 * funcs: freopen
 * procs: vscanf
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs Parameter validation: with the default invalid-parameter handler the process terminates
 * note:  Child only; exit status must be non-zero and not 77 (77 = call returned). stdin redirected to an empty file.
 *        HARNESS FIX (class F, Win10 22H2): mingw crt2.o installs a no-op invalid-parameter handler at startup, so the child survived (exit 77 instead of
 *        0xC0000409); the child now calls tc_iph_default() (tc.h) to restore the CRT default before the call under test.
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
#define INF "tc_gsf_neg_vscanf_default_handler_fatal_in_" SFX ".tmp"

TC_CASE(null_format_fatal)
{
    DWORD c = 0; int ran;
    mk(INF, "", 0);
    ran = tc_spawn_self("nf", &c, 20000);
    CHECK(ran); CHECK(c != 0); CHECK(c != 77);
    DeleteFileA(INF);
}
int main(void)
{
    tc_begin();
    if (tc_child_name() != NULL) {
        const char *volatile nf = NULL; int a = 0, r;
        tc_iph_default();  /* undo mingw startup no-op handler */
        if (!freopen(INF, "r", stdin)) return 3;
        r = vsc(nf, &a);
        (void)r;
        return 77;
    }
    TC_RUN(null_format_fatal);
    return tc_end();
}
