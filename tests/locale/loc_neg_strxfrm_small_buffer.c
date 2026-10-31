/* TC
 * id:    loc_neg_strxfrm_small_buffer
 * area:  locale
 * funcs: strxfrm wcsxfrm
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   C11 7.24.4.5
 * note:  Return value >= n signals truncation (C standard); the exact value 5 relies on the C-locale identity transform (observed); buffer contents indeterminate and not read. Runs in the C locale.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>

static const char *volatile v_s = "hello";
static const wchar_t *volatile v_ws = L"hello";

TC_CASE(strxfrm_ret_ge_n) { char b[8]; size_t r = strxfrm(b, v_s, 3); CHECK_EQ_U(r, 5); CHECK(r >= 3); }
TC_CASE(strxfrm_zero_n_null) { CHECK_EQ_U(strxfrm(NULL, v_s, 0), 5); }
TC_CASE(wcsxfrm_ret_ge_n) { wchar_t b[8]; size_t r = wcsxfrm(b, v_ws, 3); CHECK_EQ_U(r, 5); CHECK(r >= 3); }
TC_CASE(no_iph)
{
    char b[8]; wchar_t w[8];
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0;
    strxfrm(b, v_s, 3); strxfrm(NULL, v_s, 0); wcsxfrm(w, v_ws, 3); wcsxfrm(NULL, v_ws, 0);
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    TC_RUN(strxfrm_ret_ge_n); TC_RUN(strxfrm_zero_n_null); TC_RUN(wcsxfrm_ret_ge_n); TC_RUN(no_iph);
    return tc_end();
}
