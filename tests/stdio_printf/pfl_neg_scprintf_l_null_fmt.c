/* TC
 * id:    pfl_neg_scprintf_l_null_fmt
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscprintf_l _vscwprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _scprintf_l, _scwprintf_l, _vscprintf_l, _vscwprintf_l ("Parameter validation": NULL format -> EINVAL, -1)
 * note:  Extends pfl_neg_vsprintf_l_null (only _vscprintf_l) to the wide and variadic length-counting forms.
 *        The invalid-specifier case is a separate probe: pfl_neg_scprintf_l_bad_spec_probe.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

static _locale_t loc;
static const char * volatile v_f;
static const wchar_t * volatile v_wf;

TC_CASE(scprintf_l_null)
{
    int r; v_f = NULL; tc_iph_count = 0; errno = 0;
    r = pl_scprintf_l(v_f, loc, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(scwprintf_l_null)
{
    int r; v_wf = NULL; tc_iph_count = 0; errno = 0;
    r = pl_scwprintf_l(v_wf, loc, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
static int vscw(const wchar_t *f, ...)
{ va_list ap; int r; va_start(ap, f); r = pl_vscwprintf_l(f, loc, ap); va_end(ap); return r; }
TC_CASE(vscwprintf_l_null)
{
    int r; v_wf = NULL; tc_iph_count = 0; errno = 0;
    r = vscw(v_wf, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(valid_exact_length)
{
    tc_iph_count = 0;
    CHECK_EQ_I(pl_scprintf_l("%d-%s", loc, 123, "ab"), 6);
    CHECK_EQ_I(pl_scwprintf_l(L"%d-%ls", loc, 123, L"abc"), 7);
    CHECK_EQ_I(tc_iph_count, 0);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::main _create_locale(C) failed\n"); return 1; }
    TC_RUN(scprintf_l_null); TC_RUN(scwprintf_l_null); TC_RUN(vscwprintf_l_null); TC_RUN(valid_exact_length);
    _free_locale(loc);
    return tc_end();
}
