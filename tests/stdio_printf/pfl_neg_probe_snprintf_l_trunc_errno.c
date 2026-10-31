/* TC
 * id:    pfl_neg_probe_snprintf_l_trunc_errno
 * area:  stdio_printf
 * funcs: _snprintf_l _vsnwprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf, _snwprintf (truncation returns a negative value; errno/invalid-parameter not documented for it)
 * note:  PROBE: legacy-style truncation of _snprintf_l / _snwprintf_l returns -1 but neither sets errno nor calls the
 *        invalid-parameter handler (unlike the _s variants). errno is pre-set to a sentinel and must remain unchanged.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_s = "0123456789";
static volatile const wchar_t *v_w = L"0123456789";
static int have_iph;

/* _snwprintf_l is an inline/varargs wrapper in the ucrt headers and absent from mingw headers: go through the vsnwprintf_l level. */
static int snw(wchar_t *b, size_t n, const wchar_t *f, _locale_t l, ...)
{
    va_list ap; int r;
    va_start(ap, l);
#if TC_CRT_VER >= 140
    r = __stdio_common_vswprintf(4ULL, b, n, f, l, ap);
#else
    r = _vsnwprintf_l(b, n, f, l, ap);
#endif
    va_end(ap);
    return r < 0 ? -1 : r;
}

TC_CASE(narrow_errno_zero)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0; errno = 0;
    r = _snprintf_l(b, 5, "%s", l, (const char *)v_s);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}
TC_CASE(wide_errno_zero)
{
    wchar_t b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0; errno = 0;
    r = snw(b, 5, L"%ls", l, (const wchar_t *)v_w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}
TC_CASE(narrow_errno_erange_preserved)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0; errno = ERANGE;
    r = _snprintf_l(b, 5, "%s", l, (const char *)v_s);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}
TC_CASE(wide_errno_erange_preserved)
{
    wchar_t b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0; errno = ERANGE;
    r = snw(b, 5, L"%ls", l, (const wchar_t *)v_w);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(narrow_errno_zero); TC_RUN(wide_errno_zero); TC_RUN(narrow_errno_erange_preserved); TC_RUN(wide_errno_erange_preserved);
    return tc_end();
}
