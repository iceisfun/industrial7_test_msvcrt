/* TC
 * id:    pfx_neg_probe_fprintf_invalid_conv_partial_output
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen fclose
 * crt:   >=100
 * tier:  p0
 * conf:  probe
 * ref:   MS docs printf format specification; "Parameter validation"
 * note:  PROBES which bytes reach the file when fprintf("abc%yxyz") fails on the invalid conversion '%y': the literal text before it
 *        ("abc") is expected to have been emitted (exactly "abc", length 3) because output is produced incrementally, and nothing after.
 *        Also probes that the stream remains usable without clearerr, and that a failure at the very start leaves 0 bytes.
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
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    (void)b; fclose(f);
    DeleteFileA(TMP);
}
TC_CASE(partial_prefix_content)   /* isolated guess: the bytes that reach the file */
{
    FILE *f = fopen(TMP, "w"); char b[32]; CHECK(f != NULL); if (!f) return;
    (void)fprintf(f, g_fmt, 1);
    (void)fclose(f);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 3); CHECK_STR(b, "abc");
    DeleteFileA(TMP);
}
TC_CASE(stream_usable_appends)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    r = fprintf(f, g_fmt, 1); CHECK_EQ_I(r, -1);
    r = fprintf(f, "Z"); CHECK_EQ_I(r, 1);
    CHECK_EQ_I(fclose(f), 0);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 4); CHECK_STR(b, "abcZ");
    DeleteFileA(TMP);
}
TC_CASE(failure_at_start_zero_bytes)
{
    FILE *f = fopen(TMP, "w"); int r; char b[32]; CHECK(f != NULL); if (!f) return;
    tc_iph_count = 0; errno = 0; r = fprintf(f, g_y, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL);
    (void)fclose(f);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 0);
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL no iph api\n"); return 1; }
    TC_RUN(partial_prefix); TC_RUN(partial_prefix_content); TC_RUN(stream_usable_appends); TC_RUN(failure_at_start_zero_bytes);
    return tc_end();
}
