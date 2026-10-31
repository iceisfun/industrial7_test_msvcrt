/* TC
 * id:    wcs_wcsxfrm_basic
 * area:  wstring
 * funcs: wcsxfrm wcscoll
 * crt:   all
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.29.4.4.4 wcsxfrm; C locale transform is the identity (same length)
 */
#include "tc.h"
#include <wchar.h>

TC_CASE(length_and_copy)
{
    const wchar_t * volatile s = L"hello"; wchar_t d[16]; size_t r; int i;
    for (i = 0; i < 16; i++) d[i] = 0x7777;
    r = wcsxfrm(d, s, 16);
    CHECK_EQ_U(r, 5);
    CHECK_WSTR(d, L"hello");
}
TC_CASE(n_zero_null_dest)
{
    const wchar_t * volatile s = L"hello"; wchar_t * volatile d = NULL;
    CHECK_EQ_U(wcsxfrm(d, s, 0), 5);
}
TC_CASE(truncation_return)
{
    const wchar_t * volatile s = L"hello"; wchar_t d[16] = {0};
    CHECK_EQ_U(wcsxfrm(d, s, 3), 5);   /* return >= n signals truncation; contents unspecified */
    CHECK_EQ_U(wcsxfrm(d, s, 5), 5);
}
TC_CASE(empty_src)
{
    const wchar_t * volatile s = L""; wchar_t d[4] = { 0x7777, 0x7777, 0x7777, 0x7777 };
    CHECK_EQ_U(wcsxfrm(d, s, 4), 0);
    CHECK_EQ_I(d[0], 0);
}
TC_CASE(consistent_with_coll)
{
    const wchar_t * volatile a = L"apple"; const wchar_t * volatile b = L"banana";
    wchar_t xa[16], xb[16]; int c, x;
    wcsxfrm(xa, a, 16); wcsxfrm(xb, b, 16);
    c = wcscoll(a, b); x = wcscmp(xa, xb);
    CHECK((c < 0 && x < 0) || (c == 0 && x == 0) || (c > 0 && x > 0));
    c = wcscoll(b, a); x = wcscmp(xb, xa);
    CHECK((c < 0 && x < 0) || (c == 0 && x == 0) || (c > 0 && x > 0));
    c = wcscoll(a, a); x = wcscmp(xa, xa);
    CHECK(c == 0 && x == 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(length_and_copy); TC_RUN(n_zero_null_dest); TC_RUN(truncation_return); TC_RUN(empty_src); TC_RUN(consistent_with_coll);
    return tc_end();
}
