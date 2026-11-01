/* TC
 * id:    pfx_neg_probe_ftell_fflush_after_close_fd
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf ftell fflush fopen fclose _fileno _close setvbuf
 * crt:   >=80
 * tier:  p1
 * conf:  probe
 * ref:   MS docs ftell, fflush, _close (EBADF)
 * note:  PROBES stdio behaviour when the underlying fd of a stream was closed behind its back via _close(_fileno(f)): ftell -> -1/EBADF;
 *        buffered fprintf then fflush -> EOF/EBADF; fclose -> EOF without crashing. Invalid-parameter calls are tolerated (handler installed; <=1 per call).
 */
#include "tc.h"
#include <stdio.h>
#include <io.h>
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

#define TMP "tc_pfx_neg_probe_ftell_fflush_after_close_fd_" TCV ".tmp"

TC_CASE(ftell_ebadf)
{
    FILE *f = fopen(TMP, "w"); long r; CHECK(f != NULL); if (!f) return;
    setvbuf(f, NULL, _IONBF, 0);
    CHECK_EQ_I(_close(_fileno(f)), 0);
    tc_iph_count = 0; errno = 0; r = ftell(f);
    CHECK_EQ_I(r, -1L); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count <= 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(fflush_ebadf)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "x"), 1);       /* buffered */
    CHECK_EQ_I(_close(_fileno(f)), 0);
    tc_iph_count = 0; errno = 0; r = fflush(f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EBADF); CHECK(tc_iph_count <= 1);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(fclose_eof)
{
    FILE *f = fopen(TMP, "w"); int r; CHECK(f != NULL); if (!f) return;
    CHECK_EQ_I(fprintf(f, "x"), 1);
    CHECK_EQ_I(_close(_fileno(f)), 0);
    tc_iph_count = 0; r = fclose(f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(tc_iph_count, 2); /* observed Win10 22H2 19045: fclose on a stream whose fd was closed calls the handler twice (flush + close) */
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(ftell_ebadf); TC_RUN(fflush_ebadf); TC_RUN(fclose_eof);
    return tc_end();
}
