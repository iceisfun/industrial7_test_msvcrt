/* TC
 * id:    pfl_sprintf_l_no_thousands
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs printf format specification (no grouping flag); C11 7.21.6.1
 * note:  printf never inserts thousands separators in any locale. The apostrophe flag is probed in pfl_probe_sprintf_l_apostrophe_flag.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

static volatile double v_d;
static volatile int v_i = 1234567;
static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";

TC_CASE(d_1234567_c)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    _sprintf_l(b, "%d|%i|%u", l, v_i, -v_i, v_i); CHECK_STR(b, "1234567|-1234567|1234567");
    _free_locale(l);
}
TC_CASE(d_1234567_de)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    _sprintf_l(b, "%d|%i|%u", l, v_i, -v_i, v_i); CHECK_STR(b, "1234567|-1234567|1234567");
    _free_locale(l);
}
TC_CASE(f_1234567_de_only_decimal)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    v_d = 1234567.891;
    _sprintf_l(b, "%.2f", l, v_d); CHECK_STR(b, "1234567,89");
    _free_locale(l);
}
TC_CASE(f_1234567_c)
{
    char b[64]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    v_d = 1234567.891;
    _sprintf_l(b, "%.2f", l, v_d); CHECK_STR(b, "1234567.89");
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(d_1234567_c); TC_RUN(d_1234567_de); TC_RUN(f_1234567_de_only_decimal); TC_RUN(f_1234567_c);
    return tc_end();
}
