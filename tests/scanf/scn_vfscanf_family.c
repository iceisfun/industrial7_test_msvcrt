/* TC
 * id:    scn_vfscanf_family
 * area:  scanf
 * funcs: fopen fclose
 * procs: vfscanf vfwscanf __stdio_common_vfscanf __stdio_common_vfwscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  documented
 * ref:   C11 7.21.6.8 vfscanf; MS docs vfscanf, vfwscanf (C99 additions, VS2013+); MS docs "Unicode stream I/O in text and binary modes" (wide input on an ANSI-mode stream converts multibyte input; C11 leaves mixed orientation undefined)
 * note:  msvcr120 is deliberately not listed: Wine's msvcr120 has vfscanf as an unimplemented stub that aborts/hangs the runner
 *        (real msvcr120 probably exports it; the code path below handles TC_CRT_VER<140 via GetProcAddress). In ucrtbase they are inline over __stdio_common_vfscanf/vfwscanf, so the wrapper
 *        forwards to those (options 0 narrow, 2 = legacy wide specifiers for wide). Cases skip if a symbol is absent.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *vfs_t)(FILE *, const char *, va_list);
typedef int (__cdecl *vfw_t)(FILE *, const wchar_t *, va_list);
typedef int (__cdecl *cn_t)(unsigned long long, FILE *, const char *, void *, va_list);
typedef int (__cdecl *cw_t)(unsigned long long, FILE *, const wchar_t *, void *, va_list);
static vfs_t p_vfs; static vfw_t p_vfw; static cn_t p_cn; static cw_t p_cw;

static int callN(FILE *f, const char *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
#if TC_CRT_VER >= 140
    r = p_cn(0, f, fmt, NULL, ap);
#else
    r = p_vfs(f, fmt, ap);
#endif
    va_end(ap); return r;
}
static int callW(FILE *f, const wchar_t *fmt, ...)
{
    va_list ap; int r; va_start(ap, fmt);
#if TC_CRT_VER >= 140
    r = p_cw(2, f, fmt, NULL, ap);
#else
    r = p_vfw(f, fmt, ap);
#endif
    va_end(ap); return r;
}
static int have_n(void) { return TC_CRT_VER >= 140 ? p_cn != NULL : p_vfs != NULL; }
static int have_w(void) { return TC_CRT_VER >= 140 ? p_cw != NULL : p_vfw != NULL; }
#define STR_(x) #x
#define XSTR_(x) STR_(x)
static const char *fname = "tc_scn_vfscanf_family_" XSTR_(TC_CRT_VER) ".tmp";
static FILE *mk(const char *content, DWORD len)
{
    HANDLE h = CreateFileA(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); DWORD w;
    if (h == INVALID_HANDLE_VALUE) return NULL;
    WriteFile(h, content, len, &w, NULL); CloseHandle(h);
    return fopen(fname, "r");
}
static void done(FILE *f) { if (f) fclose(f); DeleteFileA(fname); }

TC_CASE(ints)
{
    FILE *f; int a = 0, b = 0, n;
    if (!have_n()) TC_SKIP("vfscanf not available");
    f = mk("11 22\n", 6); CHECK(f != NULL); if (!f) return;
    n = callN(f, "%d %d", &a, &b);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 11); CHECK_EQ_I(b, 22);
    done(f);
}
TC_CASE(eof)
{
    FILE *f; int a = 5, n;
    if (!have_n()) TC_SKIP("vfscanf not available");
    f = mk("", 0); CHECK(f != NULL); if (!f) return;
    n = callN(f, "%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 5);
    done(f);
}
TC_CASE(second_read_eof)
{
    FILE *f; int a = 0, n;
    if (!have_n()) TC_SKIP("vfscanf not available");
    f = mk("9", 1); CHECK(f != NULL); if (!f) return;
    n = callN(f, "%d", &a); CHECK_EQ_I(n, 1); CHECK_EQ_I(a, 9);
    n = callN(f, "%d", &a); CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 9);
    done(f);
}
TC_CASE(wide)
{
    FILE *f; int a = 0, n; wchar_t w[16];
    if (!have_w()) TC_SKIP("vfwscanf not available");
    f = mk("33 word\n", 8); CHECK(f != NULL); if (!f) return;
    n = callW(f, L"%d %ls", &a, w);
    CHECK_EQ_I(n, 2); CHECK_EQ_I(a, 33); CHECK_WSTR(w, L"word");
    done(f);
}
TC_CASE(wide_eof)
{
    FILE *f; int a = 5, n;
    if (!have_w()) TC_SKIP("vfwscanf not available");
    f = mk("", 0); CHECK(f != NULL); if (!f) return;
    n = callW(f, L"%d", &a);
    CHECK_EQ_I(n, EOF); CHECK_EQ_I(a, 5);
    done(f);
}
int main(void)
{
    tc_begin();
    p_vfs = (vfs_t)tc_crt_proc("vfscanf"); p_vfw = (vfw_t)tc_crt_proc("vfwscanf");
    p_cn = (cn_t)tc_crt_proc("__stdio_common_vfscanf"); p_cw = (cw_t)tc_crt_proc("__stdio_common_vfwscanf");
    TC_RUN(ints); TC_RUN(eof); TC_RUN(second_read_eof); TC_RUN(wide); TC_RUN(wide_eof);
    return tc_end();
}
