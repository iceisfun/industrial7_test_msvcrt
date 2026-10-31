/* TC
 * id:    loc_strxfrm_basic
 * area:  locale
 * funcs: strxfrm wcsxfrm _strxfrm_l _wcsxfrm_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  positive
 * conf:  observed
 * ref:   C11 7.24.4.5; MS docs strxfrm
 * note:  In the C locale strxfrm is the identity transform (observed on Windows CRTs; the C standard only requires ordering consistency with strcoll).
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>

static const char *volatile v_s = "hello";
static const char *volatile v_e = "";
static const wchar_t *volatile v_ws = L"hello";
static _locale_t loc;

TC_CASE(length_query_c) { CHECK_EQ_U(strxfrm(NULL, v_s, 0), 5); }
TC_CASE(copy_c)
{
    char b[16]; memset(b, 'x', sizeof b);
    CHECK_EQ_U(strxfrm(b, v_s, sizeof b), 5);
    CHECK_STR(b, "hello");
}
TC_CASE(empty_src)
{
    char b[4]; memset(b, 'x', sizeof b);
    CHECK_EQ_U(strxfrm(b, v_e, sizeof b), 0);
    CHECK_EQ_I(b[0], 0);
    CHECK_EQ_U(strxfrm(NULL, v_e, 0), 0);
}
TC_CASE(wide_variants)
{
    wchar_t b[16]; int i; for (i = 0; i < 16; i++) b[i] = L'x';
    CHECK_EQ_U(wcsxfrm(NULL, v_ws, 0), 5);
    CHECK_EQ_U(wcsxfrm(b, v_ws, 16), 5);
    CHECK_WSTR(b, L"hello");
}
TC_CASE(l_variants)
{
    char b[16]; wchar_t w[16];
    CHECK_EQ_U(_strxfrm_l(NULL, v_s, 0, loc), 5);
    CHECK_EQ_U(_strxfrm_l(b, v_s, sizeof b, loc), 5);
    CHECK_STR(b, "hello");
    CHECK_EQ_U(_wcsxfrm_l(NULL, v_ws, 0, loc), 5);
    CHECK_EQ_U(_wcsxfrm_l(w, v_ws, 16, loc), 5);
    CHECK_WSTR(w, L"hello");
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(length_query_c); TC_RUN(copy_c); TC_RUN(empty_src); TC_RUN(wide_variants); TC_RUN(l_variants);
    _free_locale(loc);
    return tc_end();
}
