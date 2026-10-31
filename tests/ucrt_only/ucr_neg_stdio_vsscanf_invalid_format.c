/* TC
 * id:    ucr_neg_stdio_vsscanf_invalid_format
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf __stdio_common_vswscanf
 * crt:   win10
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.2p13 (invalid conversion specification is undefined); MS docs scanf type field characters, Parameter validation
 * note:  PROBES that scanf with an invalid conversion (%y), unterminated scanset ("%[") and dangling suppression ("%*")
 *        never reports a successful assignment (return <= 0; EOF if the invalid-parameter handler path is taken) and leaves
 *        the target untouched. Narrow and wide entry points. Handler installed so the invalid-parameter path returns.
 */
#include "tc.h"
#include <stdarg.h>
#include <string.h>

typedef int (__cdecl *vsscanf_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
typedef int (__cdecl *vswscanf_t)(unsigned long long, const wchar_t *, size_t, const wchar_t *, void *, va_list);
static vsscanf_t p_vss;
static vswscanf_t p_vsws;

static int xs(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vss(0ULL, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }
static int xw(const wchar_t *in, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vsws(0ULL, in, (size_t)-1, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(bad_conv_y)
{
    int a = 77; int r = xs("1", "%y", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 77);
}
TC_CASE(unterminated_scanset)
{
    char b[8]; int r;
    memset(b, 'z', sizeof b);
    r = xs("abc", "%[", b);
    CHECK(r <= 0); CHECK(b[0] == 'z');
}
TC_CASE(dangling_suppress)
{
    int a = 77; int r = xs("1", "%*", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 77);
}
TC_CASE(wide_bad_conv_y)
{
    int a = 77; int r = xw(L"1", L"%y", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 77);
}
TC_CASE(wide_unterminated_scanset)
{
    wchar_t b[8]; int r, i;
    for (i = 0; i < 8; i++) b[i] = L'z';
    r = xw(L"abc", L"%[", b);
    CHECK(r <= 0); CHECK(b[0] == L'z');
}
TC_CASE(wide_dangling_suppress)
{
    int a = 77; int r = xw(L"1", L"%*", &a);
    CHECK(r <= 0); CHECK_EQ_I(a, 77);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    p_vss = (vsscanf_t)tc_crt_proc("__stdio_common_vsscanf");
    p_vsws = (vswscanf_t)tc_crt_proc("__stdio_common_vswscanf");
    if (!p_vss || !p_vsws) { tc_write("TC FAIL " TC_TEST_ID "::__proc missing\n"); return 1; }
    TC_RUN(bad_conv_y); TC_RUN(unterminated_scanset); TC_RUN(dangling_suppress);
    TC_RUN(wide_bad_conv_y); TC_RUN(wide_unterminated_scanset); TC_RUN(wide_dangling_suppress);
    return tc_end();
}
