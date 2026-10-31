/* TC
 * id:    pfm_2_p_variants_conv_gaps
 * area:  stdio_printf
 * funcs: _sprintf_p _fprintf_p _printf_p _swprintf_p
 * crt:   >=90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs printf_p, sprintf_p (positional parameters: %n$ must be used for every conversion; an argument may be reused)
 * note:  The wide cases use only %hs/%ls/%lc so they do not depend on the legacy/ISO wide-specifier mode (%s/%S/%c/%C change meaning).
 *        Uses include/pfq_shim.h: on ucrtbase the _p functions are SDK inlines over __stdio_common_v*_p (options 0). _printf_p writes
 *        to stdout redirected once with freopen to a temp file (STD_OUTPUT_HANDLE saved/restored). _fprintf_p goes to an ANSI
 *        text-mode temp file. Older msvcr80 is left out: mingw's import library for it lacks the _p functions.
 */
#include "tc.h"
#include "pfq_shim.h"

#define XS_(x) #x
#define XS(x) XS_(x)
#define FN  "tc_pfm_2_pv_" XS(TC_CRT_VER) ".tmp"
#define FN2 "tc_pfm_2_pv_out_" XS(TC_CRT_VER) ".tmp"

static volatile int v5 = 5, v255 = 255, vc = 'c';
static volatile double v15 = 1.5, v25 = 2.5;
static const char *volatile ns = "narrow";
static const wchar_t *volatile ws = L"wide";

static size_t slurp(const char *fn, char *b, size_t max)
{
    FILE *f = fopen(fn, "rb"); size_t n = 0;
    if (f) { n = fread(b, 1, max - 1, f); fclose(f); }
    b[n] = 0; return n;
}

TC_CASE(sprintf_p_reorder)  { char b[64]; int r = pq_sprintf_p(b, sizeof b, "%2$s %1$i", v5, ns); CHECK_STR(b, "narrow 5"); CHECK_EQ_I(r, 8); }
TC_CASE(sprintf_p_reuse_radix)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%1$X %1$x %1$o %1$u", v255);
    CHECK_STR(b, "FF ff 377 255"); CHECK_EQ_I(r, 13);
}
TC_CASE(sprintf_p_c_C_S_p)
{
    char b[64]; int r = pq_sprintf_p(b, sizeof b, "%3$S|%1$c|%2$C|%4$p", vc, (int)L'w', ws, (void *)(size_t)0x1234);
    CHECK_STR(b, TC_X64 ? "wide|c|w|0000000000001234" : "wide|c|w|00001234"); CHECK_EQ_I(r, TC_X64 ? 25 : 17);
}
TC_CASE(swprintf_p_hs_lc_ls)
{
    wchar_t b[64]; int r = pq_swprintf_p(b, 64, L"%2$hs %1$lc %3$ls", vc, ns, ws);
    CHECK_WSTR(b, L"narrow c wide"); CHECK_EQ_I(r, 13);
}
TC_CASE(swprintf_p_floats)
{
    wchar_t b[96]; int r = pq_swprintf_p(b, 96, L"%2$f|%1$G|%2$E", v15, v25);
#if TC_CRT_VER >= 140
    CHECK_WSTR(b, L"2.500000|1.5|2.500000E+00"); CHECK_EQ_I(r, 25);
#else
    CHECK_WSTR(b, L"2.500000|1.5|2.500000E+000"); CHECK_EQ_I(r, 26);
#endif
}
TC_CASE(fprintf_p_file)
{
    FILE *f = fopen(FN, "w"); char b[64]; int r;
    CHECK(f != NULL); if (!f) return;
    r = pq_fprintf_p(f, "%2$g %1$e", v15, v25); fclose(f);
    slurp(FN, b, sizeof b);
#if TC_CRT_VER >= 140
    CHECK_STR(b, "2.5 1.500000e+00"); CHECK_EQ_I(r, 16);
#else
    CHECK_STR(b, "2.5 1.500000e+000"); CHECK_EQ_I(r, 17);
#endif
    remove(FN);
}
static int g_ok, r_pp;
static HANDLE g_dup;
TC_CASE(redirect)
{
    HANDLE orig = GetStdHandle(STD_OUTPUT_HANDLE);
    DuplicateHandle(GetCurrentProcess(), orig, GetCurrentProcess(), &g_dup, 0, FALSE, DUPLICATE_SAME_ACCESS);
    g_ok = freopen(FN2, "wb", stdout) != NULL;
    if (g_dup) SetStdHandle(STD_OUTPUT_HANDLE, g_dup);
    CHECK(g_ok);
}
TC_CASE(printf_p_count)
{
    char b[64]; size_t n;
    if (!g_ok) TC_SKIP("no redirect");
    r_pp = pq_printf_p("%3$s-%1$d-%2$d", v5, v255, ns); fflush(stdout);
    n = slurp(FN2, b, sizeof b);
    CHECK_EQ_I(r_pp, 12); CHECK_EQ_U(n, 12); CHECK_STR(b, "narrow-5-255");
    fclose(stdout);
    if (g_dup) SetStdHandle(STD_OUTPUT_HANDLE, g_dup);
    remove(FN2);
}
int main(void)
{
    tc_begin();
    TC_RUN(sprintf_p_reorder); TC_RUN(sprintf_p_reuse_radix); TC_RUN(sprintf_p_c_C_S_p); TC_RUN(swprintf_p_hs_lc_ls);
    TC_RUN(swprintf_p_floats); TC_RUN(fprintf_p_file); TC_RUN(redirect); TC_RUN(printf_p_count);
    return tc_end();
}
