/* TC
 * id:    pfl_sprintf_l_c_locale_double
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf, _sprintf_l, _create_locale; C11 7.21.6.1
 * note:  %e exponent width is 2 digits on ucrt but 3 digits by default on pre-2015 CRTs, so the e case accepts both except on ucrt. inf text only asserted on ucrt ("inf").
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile double v_d;
static const char * volatile v_c = "C";

TC_CASE(f_point)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = 3.5;
    CHECK_EQ_I(_sprintf_l(b, "%f", l, v_d), 8); CHECK_STR(b, "3.500000");
    v_d = 0.25;
    _sprintf_l(b, "%.2f", l, v_d); CHECK_STR(b, "0.25");
    _free_locale(l);
}
TC_CASE(e_exp)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = 1234.5;
    _sprintf_l(b, "%e", l, v_d);
#ifdef TC_UCRT
    CHECK_STR(b, "1.234500e+03");
#else
    CHECK(tc_strequ(b, "1.234500e+03") || tc_strequ(b, "1.234500e+003"));
#endif
    _free_locale(l);
}
TC_CASE(g_short)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = 0.5; _sprintf_l(b, "%g", l, v_d); CHECK_STR(b, "0.5");
    v_d = 123456.0; _sprintf_l(b, "%g", l, v_d); CHECK_STR(b, "123456");
    v_d = 0.0001; _sprintf_l(b, "%g", l, v_d); CHECK_STR(b, "0.0001");
    _free_locale(l);
}
TC_CASE(width_prec_double)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = 3.14159;
    _sprintf_l(b, "[%8.3f][%-8.2f][%+.1f][%08.2f]", l, v_d, v_d, v_d, v_d);
    CHECK_STR(b, "[   3.142][3.14    ][+3.1][00003.14]");
    _free_locale(l);
}
TC_CASE(inf_nan_text)
{
#ifdef TC_UCRT
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    volatile double z = 0.0;
    if (!l) TC_SKIP("no C locale");
    v_d = 1.0 / z;
    _sprintf_l(b, "%f", l, v_d); CHECK_STR(b, "inf");
    v_d = -v_d;
    _sprintf_l(b, "%f", l, v_d); CHECK_STR(b, "-inf");
    _free_locale(l);
#else
    TC_SKIP("inf text differs on pre-ucrt CRTs");
#endif
}
TC_CASE(neg_zero)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = -0.0;
    _sprintf_l(b, "%f", l, v_d); CHECK_STR(b, "-0.000000");
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(f_point); TC_RUN(e_exp); TC_RUN(g_short); TC_RUN(width_prec_double); TC_RUN(inf_nan_text); TC_RUN(neg_zero);
    return tc_end();
}
