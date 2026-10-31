/* TC
 * id:    pfx_probe_neg_readonly_errno_ebadf
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fwrite fputs fopen fclose
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   MS docs errno values (EBADF); not documented per-call for stdio
 * note:  Probing errno == EBADF after writing to a stream opened "r" (fprintf, fwrite, fputs), and fwrite returning 0 / fputs returning EOF.
 */
#include "tc.h"
#define TC_S_(x) #x
#define TC_S(x) TC_S_(x)
#define TCV TC_S(TC_CRT_VER)
#include <stdio.h>
#include <errno.h>

#define TMP "tc_pfx_probe_neg_readonly_errno_ebadf_" TCV ".tmp"
static const char *volatile g_fmt = "hello %d";
static const char *volatile g_s = "abc";

static FILE *open_ro(void)
{
    DWORD n; HANDLE h = CreateFileA(TMP, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (h != INVALID_HANDLE_VALUE) { WriteFile(h, "orig", 4, &n, NULL); CloseHandle(h); }
    return fopen(TMP, "r");
}
TC_CASE(fprintf_errno)
{
    FILE *f = open_ro(); int r;
    CHECK(f != NULL); if (!f) return;
    errno = 0; r = fprintf(f, g_fmt, 1);
    CHECK(r < 0); CHECK_EQ_I(errno, EBADF);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(fwrite_errno)
{
    FILE *f = open_ro(); size_t r;
    CHECK(f != NULL); if (!f) return;
    errno = 0; r = fwrite(g_s, 1, 3, f);
    CHECK_EQ_U(r, 0); CHECK_EQ_I(errno, EBADF);
    fclose(f); DeleteFileA(TMP);
}
TC_CASE(fputs_eof)
{
    FILE *f = open_ro(); int r;
    CHECK(f != NULL); if (!f) return;
    errno = 0; r = fputs(g_s, f);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EBADF);
    fclose(f); DeleteFileA(TMP);
}
int main(void) { tc_begin(); TC_RUN(fprintf_errno); TC_RUN(fwrite_errno); TC_RUN(fputs_eof); return tc_end(); }
