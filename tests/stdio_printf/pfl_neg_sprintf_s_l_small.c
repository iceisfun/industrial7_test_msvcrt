/* TC
 * id:    pfl_neg_sprintf_s_l_small
 * area:  stdio_printf
 * funcs: _sprintf_s_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s, _sprintf_s_l (buffer too small: invalid parameter handler, ERANGE, buffer[0] = 0)
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_s = "hello";
static int have_iph;

TC_CASE(too_small_ret_minus1)
{
    char b[5]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l || !have_iph) TC_SKIP("setup");
    memset(b, 'X', sizeof b);
    CHECK_EQ_I(_sprintf_s_l(b, sizeof b, "%s", l, (const char *)v_s), -1);
    _free_locale(l);
}
TC_CASE(errno_erange)
{
    char b[5]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l || !have_iph) TC_SKIP("setup");
    errno = 0;
    _sprintf_s_l(b, sizeof b, "%s", l, (const char *)v_s);
    CHECK_EQ_I(errno, ERANGE);
    _free_locale(l);
}
TC_CASE(iph_count_one)
{
    char b[5]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0;
    _sprintf_s_l(b, sizeof b, "%s", l, (const char *)v_s);
    CHECK_EQ_I(tc_iph_count, 1);
    _free_locale(l);
}
TC_CASE(buf0_zero_after_fail)
{
    char b[5]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l || !have_iph) TC_SKIP("setup");
    memset(b, 'X', sizeof b);
    _sprintf_s_l(b, sizeof b, "%s", l, (const char *)v_s);
    CHECK_EQ_I(b[0], 0);
    _free_locale(l);
}
TC_CASE(exact_fit_ok_n_minus_1)
{
    char b[6]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0;
    CHECK_EQ_I(_sprintf_s_l(b, sizeof b, "%s", l, (const char *)v_s), 5);
    CHECK_STR(b, "hello"); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(too_small_ret_minus1); TC_RUN(errno_erange); TC_RUN(iph_count_one); TC_RUN(buf0_zero_after_fail); TC_RUN(exact_fit_ok_n_minus_1);
    return tc_end();
}
