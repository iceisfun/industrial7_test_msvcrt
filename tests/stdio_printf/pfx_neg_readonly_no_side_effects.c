/* TC
 * id:    pfx_neg_readonly_no_side_effects
 * area:  stdio_printf
 * kind:  negative
 * funcs: fprintf fopen ftell fclose fseek
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.1p3 (fprintf returns negative on output error), 7.21.5.3; MS docs fprintf ("returns a negative value on an output error")
 * note:  PROBES (output to an input-only stream is not defined by C11; MS docs do not specify it): A failed fprintf on a stream opened "r" must leave the file bytes and the stream position untouched. Only the sign of the
 *        return is asserted (not errno, not the invalid-parameter handler).
 */
#include "tc.h"
#include <stdio.h>
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

#define TMP "tc_pfx_neg_readonly_no_side_effects_" TCV ".tmp"
static const char *volatile g_fmt = "zzzz";
static const char *volatile g_empty = "";

TC_CASE(fail_pos_unchanged)
{
    FILE *f; char b[16];
    wr(TMP, "abcd", 4);
    f = fopen(TMP, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(TMP); return; }
    CHECK_EQ_I(ftell(f), 0);
    CHECK(fprintf(f, "%s", g_fmt) < 0);
    CHECK_EQ_I(ftell(f), 0);
    fclose(f);   /* return not asserted: the stream carries an error flag */
    CHECK_EQ_I(rd(TMP, b, sizeof b), 4); CHECK_STR(b, "abcd");
    DeleteFileA(TMP);
}
TC_CASE(fail_after_seek)
{
    FILE *f; char b[16];
    wr(TMP, "abcd", 4);
    f = fopen(TMP, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(TMP); return; }
    CHECK_EQ_I(fseek(f, 2, SEEK_SET), 0);
    CHECK_EQ_I(ftell(f), 2);
    CHECK(fprintf(f, "%s", g_fmt) < 0);
    CHECK_EQ_I(ftell(f), 2);
    fclose(f);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 4); CHECK_STR(b, "abcd");
    DeleteFileA(TMP);
}
TC_CASE(empty_format_file_unchanged)
{
    FILE *f; char b[16];
    wr(TMP, "abcd", 4);
    f = fopen(TMP, "r"); CHECK(f != NULL); if (!f) { DeleteFileA(TMP); return; }
    (void)fprintf(f, g_empty);   /* return value deliberately not asserted */
    fclose(f);
    CHECK_EQ_I(rd(TMP, b, sizeof b), 4); CHECK_STR(b, "abcd");
    DeleteFileA(TMP);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(fail_pos_unchanged); TC_RUN(fail_after_seek); TC_RUN(empty_format_file_unchanged);
    return tc_end();
}
