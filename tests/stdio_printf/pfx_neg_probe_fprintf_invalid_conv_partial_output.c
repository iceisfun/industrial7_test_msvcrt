/* TC
 * id:    pfx_neg_probe_fprintf_invalid_conv_partial_output
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs printf format specification; "Parameter validation"
 * note:  Observed on Windows 10 22H2 ucrtbase: fprintf does not reject the unknown conversion '%y'; the '%' is dropped and the rest is copied, so
 *        fprintf("abc%yxyz") returns 7 and the file holds "abcyxyz"; errno stays 0, no handler call; the stream stays usable; "%yxyz" returns 4 ("yxyz").
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

#define TMP "tc_pfx_neg_probe_fprintf_invalid_conv_partial_output_" TCV ".tmp"
static const char *volatile g_fmt = "abc%yxyz";
static const char *volatile g_y = "%yxyz";

TC_CASE(partial_prefix)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_fmt, 1);
    CHECK_EQ_I(r, 7); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    (void)b; fclose(f);
    DeleteFileA(TMP);
}
TC_CASE(partial_prefix_content)   /* isolated guess: the bytes that reach the file */
{
    FILE *f = fopen(TMP, "w"); char b[32]; CHECK(f != NULL); if (!f) return;
    (void)fprintf(f, g_fmt, 1);
    (void)fclose(f);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 7); CHECK_STR(b, "abcyxyz");
    DeleteFileA(TMP);
}
TC_CASE(stream_usable_appends)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    r = fprintf(f, g_fmt, 1); CHECK_EQ_I(r, 7);
    r = fprintf(f, "Z"); CHECK_EQ_I(r, 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 8); CHECK_STR(b, "abcyxyzZ");
    DeleteFileA(TMP);
}
TC_CASE(failure_at_start_zero_bytes)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_y, 1);
    CHECK_EQ_I(r, 4); CHECK_EQ_I(errno, 0);
    (void)fclose(f);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 4); CHECK_STR(b, "yxyz");
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(partial_prefix); TC_RUN(partial_prefix_content); TC_RUN(stream_usable_appends); TC_RUN(failure_at_start_zero_bytes);
    return tc_end();
}
