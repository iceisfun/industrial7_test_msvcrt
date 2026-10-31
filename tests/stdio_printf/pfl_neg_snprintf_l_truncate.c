/* TC
 * id:    pfl_neg_snprintf_l_truncate
 * area:  stdio_printf
 * funcs: _snprintf_l _create_locale _free_locale
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf: if len > count, count bytes are stored without NUL and a negative value is returned
 * note:  errno after truncation is not documented and not asserted.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_s = "0123456789";

TC_CASE(trunc_ret_minus1)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    CHECK(_snprintf_l(b, 5, "%s", l, (const char *)v_s) < 0);
    CHECK_EQ_I(_snprintf_l(b, 5, "%s", l, (const char *)v_s), -1);
    _free_locale(l);
}
TC_CASE(filled_count_chars_no_nul)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    memset(b, 'X', sizeof b);
    _snprintf_l(b, 5, "%s", l, (const char *)v_s);
    CHECK_MEM(b, "01234", 5);
    _free_locale(l);
}
TC_CASE(one_short_of_len)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    memset(b, 'X', sizeof b);
    CHECK_EQ_I(_snprintf_l(b, 9, "%s", l, (const char *)v_s), -1);
    CHECK_MEM(b, "012345678X", 10);
    _free_locale(l);
}
TC_CASE(no_write_past_count_guard_bytes)
{
    char b[24]; _locale_t l = _create_locale(LC_ALL, v_c); int i;
    if (!l) TC_SKIP("no C locale");
    memset(b, 'G', sizeof b);
    _snprintf_l(b + 4, 6, "%s", l, (const char *)v_s);
    for (i = 0; i < 4; i++) CHECK_EQ_I(b[i], 'G');
    for (i = 10; i < 24; i++) CHECK_EQ_I(b[i], 'G');
    CHECK_MEM(b + 4, "012345", 6);
    _free_locale(l);
}
TC_CASE(count_len_plus_one_not_truncated)
{
    char b[16]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l) TC_SKIP("no C locale");
    CHECK_EQ_I(_snprintf_l(b, 11, "%s", l, (const char *)v_s), 10);
    CHECK_STR(b, "0123456789");
    _free_locale(l);
}

int main(void)
{
    tc_begin();
    TC_RUN(trunc_ret_minus1); TC_RUN(filled_count_chars_no_nul); TC_RUN(one_short_of_len);
    TC_RUN(no_write_past_count_guard_bytes); TC_RUN(count_len_plus_one_not_truncated);
    return tc_end();
}
