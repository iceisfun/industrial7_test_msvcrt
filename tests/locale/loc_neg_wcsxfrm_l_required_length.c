/* TC
 * id:    loc_neg_wcsxfrm_l_required_length
 * area:  locale
 * funcs: _wcsxfrm_l _strxfrm_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.24.4.5 strxfrm, 7.29.4.4.4 wcsxfrm (return the length required; n == 0 allows a null destination)
 * note:  In the "C" locale the transformed length equals the source length; asserted as >= to stay locale-neutral. A destination
 *        too small must not be reported as success (return >= count).
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>

static const char *volatile v_abc = "abc";
static const wchar_t *volatile v_hello = L"hello";
static const char *volatile v_c = "C";

TC_CASE(length_query_count_zero)
{
    _locale_t l = _create_locale(LC_ALL, v_c); size_t r; if (!l) TC_SKIP("no locale");
    if (!tc_iph_install()) { _free_locale(l); TC_SKIP("no IPH api"); }
    tc_iph_count = 0;
    r = _strxfrm_l(NULL, v_abc, 0, l); CHECK(r >= 3);
    r = _wcsxfrm_l(NULL, v_hello, 0, l); CHECK(r >= 5);
    CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}
TC_CASE(small_buffer_returns_required)
{
    _locale_t l = _create_locale(LC_ALL, v_c); wchar_t w[4]; char c[4]; if (!l) TC_SKIP("no locale");
    CHECK(_wcsxfrm_l(w, v_hello, 2, l) >= 5);
    CHECK(_strxfrm_l(c, "hello", 2, l) >= 5);
    _free_locale(l);
}
int main(void)
{
    tc_begin();
    TC_RUN(length_query_count_zero); TC_RUN(small_buffer_returns_required);
    return tc_end();
}
