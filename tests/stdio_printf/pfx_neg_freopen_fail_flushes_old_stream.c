/* TC
 * id:    pfx_neg_freopen_fail_flushes_old_stream
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf freopen fopen
 * crt:   >=80
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.21.5.4p3 (freopen first attempts to close the file associated with the stream; failure to close is ignored); returns null pointer if the open fails
 * note:  Buffered data written with fprintf must reach the old file even though the freopen of a nonexistent directory path fails.
 *        The dead FILE* is intentionally neither used nor closed afterwards.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
static int rd(const char *p, char *b, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL); DWORD got = 0;
    if (h == INVALID_HANDLE_VALUE) { b[0] = 0; return -1; }
    ReadFile(h, b, (DWORD)(n - 1), &got, NULL); CloseHandle(h); b[got] = 0; return (int)got;
}
static void wr(const char *p, const char *d, int n)
{
    HANDLE h = CreateFileA(p, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL); DWORD w;
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, d, (DWORD)n, &w, NULL); CloseHandle(h); }
}

#define TMP "tc_pfx_neg_freopen_fail_flushes_old_stream_" TCV ".tmp"
static const char *volatile g_bad = "tc_nodir_pfx\\x";

TC_CASE(freopen_null_enoent)
{
    FILE *f = fopen(TMP, "w"), *r; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abc"), 3);
    errno = 0; r = freopen(g_bad, "w", f);
    CHECK(r == NULL); CHECK_EQ_I(errno, ENOENT);
    DeleteFileA(TMP);
}
TC_CASE(old_data_flushed)
{
    FILE *f = fopen(TMP, "w"), *r; char b[16]; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "abc"), 3);
    r = freopen(g_bad, "w", f);
    CHECK(r == NULL);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 3); CHECK_STR(b, "abc");
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(freopen_null_enoent); TC_RUN(old_data_flushed);
    return tc_end();
}
