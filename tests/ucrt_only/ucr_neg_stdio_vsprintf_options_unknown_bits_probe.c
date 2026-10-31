/* TC
 * id:    ucr_neg_stdio_vsprintf_options_unknown_bits_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsprintf __stdio_common_vsprintf_s __stdio_common_vswprintf
 * crt:   win10
 * tier:  p1
 * conf:  probe
 * ref:   <corecrt_stdio_config.h> option bits 1,2,4,8,0x10 (_CRT_INTERNAL_PRINTF_*); higher bits undefined
 * note:  PROBES: (1) an undefined option bit (1<<20) is ignored: "%d" of 42 gives "42", length 2 (both vsprintf and vsprintf_s);
 *        (2) wide printf with LEGACY_WIDE_SPECIFIERS|STANDARD_SNPRINTF (6): "%s|%S" with (L"ab", "cd") gives "ab|cd";
 *        (3) option LEGACY_MSVCRT_COMPATIBILITY (8) with %f of +inf yields "inf" or the legacy "1.#INF00" (observed value is printed).
 */
#include "tc.h"
#include <stdarg.h>
#include <string.h>

typedef int (__cdecl *vsp_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vswp_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsp_t p_vsp, p_vsps;
static vswp_t p_vswp;

static int xp(vsp_t f, unsigned long long o, char *b, size_t n, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = f(o, b, n, fmt, NULL, ap); va_end(ap); return r; }
static int xw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vswp(o, b, n, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(unknown_bit_vsprintf)
{
    char b[32]; int r;
    memset(b, 'z', sizeof b);
    r = xp(p_vsp, 1ULL << 20, b, sizeof b, "%d", 42);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "42");
}
TC_CASE(unknown_bit_vsprintf_s)
{
    char b[32]; int r;
    memset(b, 'z', sizeof b);
    r = xp(p_vsps, 1ULL << 20, b, sizeof b, "%d", 42);
    CHECK_EQ_I(r, 2); CHECK_STR(b, "42");
}
TC_CASE(legacy_wide_plus_std_snprintf)
{
    wchar_t b[32]; int r;
    r = xw(6ULL, b, 32, L"%s|%S", L"ab", "cd");
    CHECK_EQ_I(r, 5); CHECK_WSTR(b, L"ab|cd");
}
TC_CASE(legacy_msvcrt_inf)
{
    char b[32]; int r; volatile double z = 0.0, inf = 1.0 / z;
    memset(b, 0, sizeof b);
    r = xp(p_vsp, 8ULL, b, sizeof b, "%f", inf);
    tc_write("TC NOTE legacy inf="); tc_write(r > 0 ? b : "(fail)"); tc_write("\n");
    CHECK(tc_strequ(b, "inf") || tc_strequ(b, "1.#INF00"));
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsp = (vsp_t)tc_crt_proc("__stdio_common_vsprintf");
    p_vsps = (vsp_t)tc_crt_proc("__stdio_common_vsprintf_s");
    p_vswp = (vswp_t)tc_crt_proc("__stdio_common_vswprintf");
    if (!p_vsp || !p_vsps || !p_vswp) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(unknown_bit_vsprintf); TC_RUN(unknown_bit_vsprintf_s); TC_RUN(legacy_wide_plus_std_snprintf); TC_RUN(legacy_msvcrt_inf);
    return tc_end();
}
