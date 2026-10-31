/* TC
 * id:    loc_neg_wcsxfrm_l_args
 * area:  locale
 * funcs: _wcsxfrm_l _strxfrm_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strxfrm, _strxfrm_l, wcsxfrm (parameter validation)
 * note:  PROBE: NULL source, or NULL dest with count>0, set errno EINVAL and invoke the invalid-parameter handler.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <errno.h>
#include <locale.h>

static const char *volatile v_null_s;
static const wchar_t *volatile v_null_w;
static char *volatile v_null_b;
static wchar_t *volatile v_null_wb;
static const char *volatile v_abc = "abc";
static const wchar_t *volatile v_hello = L"hello";
static const char *volatile v_c = "C";

TC_CASE(null_source)
{
    char b[8]; _locale_t l = _create_locale(LC_ALL, v_c); if (!l) TC_SKIP("no locale");
    if (!tc_iph_install()) { _free_locale(l); TC_SKIP("no IPH api"); }
    tc_iph_count = 0; errno = 0; _strxfrm_l(b, v_null_s, 8, l);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    _free_locale(l);
}
TC_CASE(null_dest_nonzero_count)
{
    _locale_t l = _create_locale(LC_ALL, v_c); if (!l) TC_SKIP("no locale");
    if (!tc_iph_install()) { _free_locale(l); TC_SKIP("no IPH api"); }
    tc_iph_count = 0; errno = 0; _wcsxfrm_l(v_null_wb, v_hello, 4, l);
    CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    errno = 0; _strxfrm_l(v_null_b, v_abc, 4, l); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 2);
    _free_locale(l);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_source); TC_RUN(null_dest_nonzero_count);
    return tc_end();
}
