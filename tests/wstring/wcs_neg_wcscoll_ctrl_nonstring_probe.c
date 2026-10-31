/* TC
 * id:    wcs_neg_wcscoll_ctrl_nonstring_probe
 * area:  wstring
 * funcs: wcscoll _wcsicoll wcsxfrm setlocale
 * crt:   >=80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.11.1.1 (setlocale returns a null pointer if the request cannot be honored; locale unchanged); MS docs setlocale
 * note:  Probes that a failed setlocale(LC_ALL, "xx_NOSUCH") leaves the C locale in effect for the wide collation functions.
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>

static const char *volatile vbad = "xx_NOSUCH";
static const wchar_t *volatile va = L"a", *volatile vB = L"B";

TC_CASE(setlocale_bad_returns_null)
{
    char *r = setlocale(LC_ALL, vbad);
    CHECK(r == NULL);
    r = setlocale(LC_ALL, NULL); CHECK_STR(r, "C");
}
TC_CASE(wcscoll_unchanged) { setlocale(LC_ALL, vbad); CHECK(wcscoll(va, vB) > 0); CHECK(wcscoll(vB, va) < 0); CHECK_EQ_I(wcscoll(va, va), 0); }
TC_CASE(wcsicoll_unchanged) { setlocale(LC_ALL, vbad); CHECK(_wcsicoll(va, vB) < 0); CHECK(_wcsicoll(vB, va) > 0); }
TC_CASE(wcsxfrm_identity)
{
    wchar_t d[8]; size_t n;
    setlocale(LC_ALL, vbad);
    n = wcsxfrm(d, L"abc", 8); CHECK_EQ_U(n, 3); CHECK_WSTR(d, L"abc");
}
int main(void)
{
    tc_begin(); TC_RUN(setlocale_bad_returns_null); TC_RUN(wcscoll_unchanged); TC_RUN(wcsicoll_unchanged); TC_RUN(wcsxfrm_identity);
    setlocale(LC_ALL, "C");
    return tc_end();
}
