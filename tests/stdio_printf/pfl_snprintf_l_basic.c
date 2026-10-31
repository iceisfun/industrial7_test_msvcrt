/* TC
 * id:    pfl_snprintf_l_basic
 * area:  stdio_printf
 * funcs: _snprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _snprintf, _snprintf_l, _snwprintf (count == len leaves the buffer unterminated)
 * note:  count==0 / NULL buffer behaviour is probed in pfl_probe_snprintf_l_count_zero.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>

static const char * volatile v_c = "C";
static const char * volatile v_de = "German_Germany.1252";
static volatile double v_d = 1.5;

TC_CASE(fits_with_nul)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int n;
    if (!l) TC_SKIP("no C locale");
    memset(b, 'X', sizeof b);
    n = _snprintf_l(b, 6, "abcde", l);
    CHECK_EQ_I(n, 5); CHECK_STR(b, "abcde"); CHECK_EQ_I(b[5], 0); CHECK_EQ_I(b[6], 'X');
    _free_locale(l);
}
TC_CASE(exact_fit_no_terminator)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c); int n;
    if (!l) TC_SKIP("no C locale");
    memset(b, 'X', sizeof b);
    n = _snprintf_l(b, 5, "abcde", l);
    CHECK_EQ_I(n, 5); CHECK_MEM(b, "abcdeXXX", 8);
    _free_locale(l);
}
TC_CASE(german_comma_in_snprintf_l)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_de);
    if (!l) TC_SKIP("no German locale");
    CHECK_EQ_I(_snprintf_l(b, sizeof b, "%.1f", l, v_d), 3); CHECK_STR(b, "1,5");
    _free_locale(l);
}
TC_CASE(percent_s_c_ptrs)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c); volatile const char *s = "str";
    if (!l) TC_SKIP("no C locale");
    CHECK_EQ_I(_snprintf_l(b, sizeof b, "<%s|%c|%5s>", l, (const char *)s, 'z', (const char *)s), 13);
    CHECK_STR(b, "<str|z|  str>");
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(fits_with_nul); TC_RUN(exact_fit_no_terminator); TC_RUN(german_comma_in_snprintf_l); TC_RUN(percent_s_c_ptrs);
    return tc_end();
}
