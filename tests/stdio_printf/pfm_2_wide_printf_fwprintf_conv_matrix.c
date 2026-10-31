/* TC
 * id:    pfm_2_wide_printf_fwprintf_conv_matrix
 * area:  stdio_printf
 * funcs: fwprintf wprintf vwprintf
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fwprintf, wprintf, vwprintf; printf type field characters
 * note:  fwprintf goes to ANSI text-mode temp files (ASCII content: identity conversion), read back with fread.
 *        wprintf/vwprintf go to stdout redirected once with freopen to a temp file (STD_OUTPUT_HANDLE saved/restored as in
 *        pfq_printf_p_redirected). %n is not used (disabled by default on ucrt).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN  "tc_pfm_2_wpf_" XS(TC_CRT_VER) ".tmp"
#define FN2 "tc_pfm_2_wpf_out_" XS(TC_CRT_VER) ".tmp"

static volatile int vm42 = -42, v8 = 8, v255 = 255, vc = 'a';
static volatile unsigned vbig = 4000000000u;
static volatile double v15 = 1.5;
static const wchar_t *volatile ws = L"cd";
static const char *volatile ns = "ef";

static size_t slurp(const char *fn, char *b, size_t max)
{
    FILE *f = fopen(fn, "rb"); size_t n = 0;
    if (f) { n = fread(b, 1, max - 1, f); fclose(f); }
    b[n] = 0; return n;
}
static int vwp(const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = vwprintf(f, ap); va_end(ap); return r; }

TC_CASE(fw_ints)
{
    FILE *f = fopen(FN, "w"); char b[64]; int r; size_t n;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%d %u %o %x %X", vm42, vbig, v8, v255, v255); fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_STR(b, "-42 4000000000 10 ff FF"); CHECK_EQ_I(r, 23); CHECK_EQ_U(n, 23); remove(FN);
}
TC_CASE(fw_chars_strings)
{
    FILE *f = fopen(FN, "w"); char b[64]; int r; size_t n;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%s %S %c %C", ws, ns, vc, vc + 1); fclose(f);
    n = slurp(FN, b, sizeof b);
    CHECK_STR(b, "cd ef a b"); CHECK_EQ_I(r, 9); CHECK_EQ_U(n, 9); remove(FN);
}
TC_CASE(fw_floats)
{
    FILE *f = fopen(FN, "w"); char b[64]; int r;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"%f %g %.2e", v15, v15, v15); fclose(f);
    slurp(FN, b, sizeof b);
#if TC_CRT_VER >= 140
    CHECK_STR(b, "1.500000 1.5 1.50e+00"); CHECK_EQ_I(r, 21);
#else
    CHECK_STR(b, "1.500000 1.5 1.50e+000"); CHECK_EQ_I(r, 22);
#endif
    remove(FN);
}
TC_CASE(fw_no_percent_n_needed)
{
    FILE *f = fopen(FN, "w"); char b[64]; int r;
    CHECK(f != NULL); if (!f) return;
    r = fwprintf(f, L"100%% %5.1f%%", v15); fclose(f);
    slurp(FN, b, sizeof b);
    CHECK_STR(b, "100%   1.5%"); CHECK_EQ_I(r, 11); remove(FN);
}
static int g_ok, r_w, r_vw;
static HANDLE g_dup;
TC_CASE(redirect)
{
    HANDLE orig = GetStdHandle(STD_OUTPUT_HANDLE);
    DuplicateHandle(GetCurrentProcess(), orig, GetCurrentProcess(), &g_dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
    g_ok = freopen(FN2, "w", stdout) != NULL;
    if (g_dup) SetStdHandle(STD_OUTPUT_HANDLE, g_dup);
    CHECK(g_ok);
}
TC_CASE(wprintf_count)
{
    if (!g_ok) TC_SKIP("no redirect");
    r_w = wprintf(L"%d|%ls;", vm42, ws); fflush(stdout);
    CHECK_EQ_I(r_w, 7);
}
TC_CASE(vwprintf_count)
{
    if (!g_ok) TC_SKIP("no redirect");
    r_vw = vwp(L"%x:%s", v255, ws); fflush(stdout);
    CHECK_EQ_I(r_vw, 5);
}
TC_CASE(verify_stdout)
{
    char b[64]; size_t n;
    if (!g_ok) TC_SKIP("no redirect");
    n = slurp(FN2, b, sizeof b);
    CHECK_STR(b, "-42|cd;ff:cd"); CHECK_EQ_U(n, 12);
    fclose(stdout);
    if (g_dup) SetStdHandle(STD_OUTPUT_HANDLE, g_dup); /* closing fd 1 resets the std handle */
    remove(FN2);
}
int main(void)
{
    tc_begin();
    TC_RUN(fw_ints); TC_RUN(fw_chars_strings); TC_RUN(fw_floats); TC_RUN(fw_no_percent_n_needed);
    TC_RUN(redirect); TC_RUN(wprintf_count); TC_RUN(vwprintf_count); TC_RUN(verify_stdout);
    return tc_end();
}
