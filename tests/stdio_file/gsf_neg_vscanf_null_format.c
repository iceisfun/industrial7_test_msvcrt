/* TC
 * id:    gsf_neg_vscanf_null_format
 * area:  stdio_file
 * funcs: freopen
 * procs: vscanf
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs scanf, _scanf_l, wscanf, _wscanf_l / Parameter validation (format == NULL -> EINVAL, handler, returns EOF)
 * note:  Child process with stdin redirected to an empty file so the call can never block; handler installed.
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
#define INF "tc_gsf_neg_vscanf_null_format_in_" SFX ".tmp"

static int child(const char *c)
{
    const char *volatile nf = NULL; int r, e, a = 0;
    (void)c;
    if (!freopen(INF, "r", stdin)) return 3;
    tc_iph_install();
    errno = 0;
    r = vsc(nf, &a); e = errno;
    if (r != EOF) return 10;
    if (e != EINVAL) return 11;
    if (tc_iph_count != 1) return 12;
    return 0;
}
TC_CASE(null_format)
{
    DWORD code = 99;
    mk(INF, "", 0);
    CHECK(tc_spawn_self("nf", &code, 20000)); CHECK_EQ_U(code, 0);
    DeleteFileA(INF);
}
int main(void)
{
    const char *c;
    tc_begin();
    if ((c = tc_child_name()) != NULL) return child(c);
    TC_RUN(null_format);
    return tc_end();
}
