/* TC
 * id:    pfm_1_probe_swprintf_l_legacy
 * area:  stdio_printf
 * funcs:
 * procs: __swprintf_l
 * crt:   msvcr120 msvcr100 msvcr90
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   MS docs _swprintf_l (non-count form: buffer, format, locale, ...)
 * note:  PROBE: the VS2005+ headers map _swprintf_l to an export named __swprintf_l (double underscore), and that export exists
 *        in msvcr90/100/120 with a NULL locale meaning "current locale".
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

typedef int (__cdecl *swprintf_l_t)(wchar_t *, const wchar_t *, void *, ...);
static swprintf_l_t p_swprintf_l;

TC_CASE(exported) { CHECK(p_swprintf_l != NULL); }
TC_CASE(null_locale)
{
    wchar_t b[32]; int n, i;
    for (i = 0; i < 32; i++) b[i] = L'Z';
    if (!p_swprintf_l) TC_SKIP("__swprintf_l not exported");
    n = p_swprintf_l(b, L"%d-%ls", NULL, 12, L"zz");
    CHECK_WSTR(b, L"12-zz"); CHECK_EQ_I(n, 5); CHECK(b[6] == L'Z');
}

int main(void)
{
    tc_begin();
    p_swprintf_l = (swprintf_l_t)tc_crt_proc("__swprintf_l");
    TC_RUN(exported); TC_RUN(null_locale);
    return tc_end();
}
