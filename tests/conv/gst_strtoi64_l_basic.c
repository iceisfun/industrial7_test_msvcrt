/* TC
 * id:    gst_strtoi64_l_basic
 * area:  conv
 * funcs: _strtoi64_l _strtoui64_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtoll, _strtoll_l, strtoull, _strtoull_l, _strtoi64 family; C11 7.22.1.4
 * note:  Explicit "C" locale object from _create_locale(LC_ALL, "C").
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>

static const char *volatile v;
static _locale_t loc;

TC_CASE(decimal_ws_signs)
{
    char *e;
    v = "  \t\n 123abc"; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 123); CHECK_EQ_P(e, v + 8);
    v = "-456";  CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), -456); CHECK_EQ_P(e, v + 4);
    v = "+789";  CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 789);  CHECK_EQ_P(e, v + 4);
    v = "  +42"; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 42);  CHECK_EQ_P(e, v + 5);
    v = "0";     CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 0);    CHECK_EQ_P(e, v + 1);
}
TC_CASE(bases)
{
    char *e;
    v = "0x1aF"; CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0x1af); CHECK_EQ_P(e, v + 5);
    v = "1aF";   CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0x1af); CHECK_EQ_P(e, v + 3);
    v = "0X10";  CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 16);
    v = "777";   CHECK_EQ_I(_strtoi64_l(v, &e, 8, loc), 511);
    v = "101101"; CHECK_EQ_I(_strtoi64_l(v, &e, 2, loc), 45);
    v = "zZ";    CHECK_EQ_I(_strtoi64_l(v, &e, 36, loc), 35 * 36 + 35); CHECK_EQ_P(e, v + 2);
    v = "Az";    CHECK_EQ_I(_strtoi64_l(v, &e, 36, loc), 10 * 36 + 35);
    v = "ff";    CHECK_EQ_U(_strtoui64_l(v, &e, 16, loc), 255);
}
TC_CASE(base0)
{
    char *e;
    v = "0x1F"; CHECK_EQ_I(_strtoi64_l(v, &e, 0, loc), 31); CHECK_EQ_P(e, v + 4);
    v = "017";   CHECK_EQ_I(_strtoi64_l(v, &e, 0, loc), 15); CHECK_EQ_P(e, v + 3);
    v = "99";    CHECK_EQ_I(_strtoi64_l(v, &e, 0, loc), 99);
    v = "-0x10"; CHECK_EQ_I(_strtoi64_l(v, &e, 0, loc), -16);
    v = "0x20";  CHECK_EQ_U(_strtoui64_l(v, &e, 0, loc), 32);
    v = "010";   CHECK_EQ_U(_strtoui64_l(v, &e, 0, loc), 8);
}
TC_CASE(endptr_trailing)
{
    char *e;
    v = "12 34"; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 12); CHECK_EQ_P(e, v + 2);
    v = "12.5";  CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 12); CHECK_EQ_P(e, v + 2);
    v = "129";   CHECK_EQ_I(_strtoi64_l(v, &e, 8, loc), 10);  CHECK_EQ_P(e, v + 2);
    v = "1012";  CHECK_EQ_U(_strtoui64_l(v, &e, 2, loc), 5);  CHECK_EQ_P(e, v + 3);
    v = "7g";    CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 7);  CHECK_EQ_P(e, v + 1);
    v = "1";     CHECK_EQ_I(_strtoi64_l(v, NULL, 10, loc), 1);
}
TC_CASE(exact_limits)
{
    char *e;
    v = "9223372036854775807";  CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 9223372036854775807LL); CHECK_EQ_P(e, v + 19);
    v = "-9223372036854775808"; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), (-9223372036854775807LL - 1)); CHECK_EQ_P(e, v + 20);
    v = "18446744073709551615"; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0xFFFFFFFFFFFFFFFFULL); CHECK_EQ_P(e, v + 20);
    v = "0xFFFFFFFFFFFFFFFF";   CHECK_EQ_U(_strtoui64_l(v, &e, 16, loc), 0xFFFFFFFFFFFFFFFFULL);
    v = "0x7FFFFFFFFFFFFFFF";   CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0x7FFFFFFFFFFFFFFFLL);
    v = "0";                    CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 0);
}
TC_CASE(no_32bit_truncation)
{
    char *e;
    v = "4294967296";  CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), 4294967296LL);
    v = "-5000000000"; CHECK_EQ_I(_strtoi64_l(v, &e, 10, loc), -5000000000LL);
    v = "123456789012345"; CHECK_EQ_U(_strtoui64_l(v, &e, 10, loc), 123456789012345ULL);
    v = "1000000000000"; CHECK_EQ_I(_strtoi64_l(v, &e, 16, loc), 0x1000000000000LL);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(decimal_ws_signs); TC_RUN(bases); TC_RUN(base0); TC_RUN(endptr_trailing); TC_RUN(exact_limits); TC_RUN(no_32bit_truncation);
    _free_locale(loc);
    return tc_end();
}
