/* TC
 * id:    pfm_1_vsnprintf_swprintf_vswprintf_gap
 * area:  stdio_printf
 * funcs: _swprintf _vswprintf
 * crt:   msvcr120 msvcr100 msvcr90
 * tier:  p1
 * kind:  positive
 * conf:  documented
 * ref:   MS docs swprintf/_swprintf, vswprintf/_vswprintf (legacy non-count forms)
 * note:  Legacy non-count forms are exported by the VS2005-VS2013 runtimes. The _l variant is covered separately (probe) in
 *        pfm_1_probe_swprintf_l_legacy.
 *        vsnprintf is covered by pfm_1_probe_vsnprintf_msvcr120_trunc
 *        and pfm_1_neg_probe_vsnprintf_absent_pre120; __stdio_common_* absence by pfm_1_neg_stdio_common_vsprintf_absent.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>

static void fill(wchar_t *b, int n) { int i; for (i = 0; i < n; i++) b[i] = L'Z'; }
static int vs(wchar_t *b, const wchar_t *f, ...)
{ va_list ap; int n; va_start(ap, f); n = _vswprintf(b, f, ap); va_end(ap); return n; }

TC_CASE(swprintf_no_count)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = _swprintf(b, L"%d:%ls:%x", -7, L"ab", 171);
    CHECK_WSTR(b, L"-7:ab:ab"); CHECK_EQ_I(n, 8); CHECK(b[8] == 0); CHECK(b[9] == L'Z');
}
TC_CASE(vswprintf_legacy)
{
    wchar_t b[32]; int n; fill(b, 32);
    n = vs(b, L"[%5d][%-4ls]", 42, L"x");
    CHECK_WSTR(b, L"[   42][x   ]"); CHECK_EQ_I(n, 13); CHECK(b[14] == L'Z');
}
int main(void)
{
    tc_begin();
    TC_RUN(swprintf_no_count); TC_RUN(vswprintf_legacy);
    return tc_end();
}
