/* TC
 * id:    cv_neg_wcstol_nodigits_endptr
 * area:  conv
 * funcs: wcstol wcstoul _wcstoi64
 * procs: wcstoll wcstoull
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4p8 (no conversion: zero returned, endptr = nptr); MS docs wcstol, wcstoul
 * note:  wcstoll/wcstoull only exist from msvcr120; skipped where absent. errno on no-conversion is in cv_neg_wcsto_nodigits_errno_probe. "0x" base 16: only the value 0 is asserted.
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>

typedef long long (__cdecl *wcstoll_t)(const wchar_t *, wchar_t **, int);
typedef unsigned long long (__cdecl *wcstoull_t)(const wchar_t *, wchar_t **, int);
static wcstoll_t p_wcstoll;
static wcstoull_t p_wcstoull;
static const wchar_t *volatile v_s;

static void none(const wchar_t *s)
{
    wchar_t *e;
    v_s = s; e = (wchar_t *)1; CHECK_EQ_I(wcstol(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    v_s = s; e = (wchar_t *)1; CHECK_EQ_U(wcstoul(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    v_s = s; e = (wchar_t *)1; CHECK_EQ_I(_wcstoi64(v_s, &e, 10), 0); CHECK_EQ_P(e, s);
    if (p_wcstoll)  { v_s = s; e = (wchar_t *)1; CHECK_EQ_I(p_wcstoll(v_s, &e, 10), 0); CHECK_EQ_P(e, s); }
    if (p_wcstoull) { v_s = s; e = (wchar_t *)1; CHECK_EQ_U(p_wcstoull(v_s, &e, 10), 0); CHECK_EQ_P(e, s); }
}

TC_CASE(empty_blank_alpha) { none(L""); none(L"   "); none(L"xyz"); }
TC_CASE(sign_only)         { none(L"+"); none(L"-"); }
TC_CASE(ws_then_sign)      { none(L"  -"); none(L"\t +"); none(L"  x"); }
TC_CASE(hex_prefix_value_zero)
{
    wchar_t *e; static const wchar_t s[] = L"0x";
    v_s = s; e = (wchar_t *)1; CHECK_EQ_I(wcstol(v_s, &e, 16), 0);
    v_s = s; e = (wchar_t *)1; CHECK_EQ_U(wcstoul(v_s, &e, 16), 0);
    v_s = s; e = (wchar_t *)1; CHECK_EQ_I(_wcstoi64(v_s, &e, 16), 0);
}

int main(void)
{
    tc_begin();
    p_wcstoll = (wcstoll_t)tc_crt_proc("wcstoll");
    p_wcstoull = (wcstoull_t)tc_crt_proc("wcstoull");
    TC_RUN(empty_blank_alpha); TC_RUN(sign_only); TC_RUN(ws_then_sign); TC_RUN(hex_prefix_value_zero);
    return tc_end();
}
