/* TC
 * id:    gst_strtoimax_l_basic
 * area:  conv
 * funcs: _strtoimax_l _strtoumax_l strtoumax _create_locale _free_locale
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strtoimax, _strtoimax_l, strtoumax, _strtoumax_l; C11 7.8.2.3, 7.22.1.4
 * note:  strtoimax (non-_l) is covered by ucr_strtoimax_wcstoimax. These intmax functions are exported from msvcr120 on (not msvcr110/100).
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
extern intmax_t __cdecl _strtoimax_l(const char *, char **, int, _locale_t);
extern uintmax_t __cdecl _strtoumax_l(const char *, char **, int, _locale_t);

static const char *volatile v;
static _locale_t loc;

TC_CASE(size_sanity) { CHECK_EQ_I(sizeof(intmax_t), 8); CHECK_EQ_I(sizeof(uintmax_t), 8); }
TC_CASE(limits)
{
    char *e;
    v = "9223372036854775807";  CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), INTMAX_MAX); CHECK_EQ_P(e, v + 19);
    v = "-9223372036854775808"; CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), INTMAX_MIN); CHECK_EQ_P(e, v + 20);
    v = "18446744073709551615"; CHECK_EQ_U(_strtoumax_l(v, &e, 10, loc), UINTMAX_MAX); CHECK_EQ_P(e, v + 20);
    v = "18446744073709551615"; CHECK_EQ_U(strtoumax(v, &e, 10), UINTMAX_MAX);
}
TC_CASE(bases)
{
    char *e;
    v = "1010";  CHECK_EQ_I(_strtoimax_l(v, &e, 2, loc), 10);
    v = "0777";  CHECK_EQ_I(_strtoimax_l(v, &e, 8, loc), 511);
    v = "-1234"; CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), -1234);
    v = "0xFf";  CHECK_EQ_I(_strtoimax_l(v, &e, 16, loc), 255);
    v = "Zz";    CHECK_EQ_I(_strtoimax_l(v, &e, 36, loc), 1295);
    v = "zz";    CHECK_EQ_U(_strtoumax_l(v, &e, 36, loc), 1295);
    v = "0x1F";  CHECK_EQ_I(_strtoimax_l(v, &e, 0, loc), 31);
    v = "017";   CHECK_EQ_I(_strtoimax_l(v, &e, 0, loc), 15);
    v = "17";    CHECK_EQ_I(_strtoimax_l(v, &e, 0, loc), 17);
    v = "0x1F";  CHECK_EQ_U(strtoumax(v, &e, 0), 31);
    v = "017";   CHECK_EQ_U(_strtoumax_l(v, &e, 0, loc), 15);
}
TC_CASE(endptr)
{
    char *e;
    v = "  123abc"; CHECK_EQ_I(_strtoimax_l(v, &e, 10, loc), 123); CHECK_EQ_P(e, v + 5);
    v = "99 88";    CHECK_EQ_U(_strtoumax_l(v, &e, 10, loc), 99);  CHECK_EQ_P(e, v + 2);
    v = "7z";       CHECK_EQ_U(strtoumax(v, &e, 10), 7);           CHECK_EQ_P(e, v + 1);
}
TC_CASE(strtoumax_matches)
{
    char *e1, *e2;
    v = "  +42"; CHECK_EQ_U(strtoumax(v, &e1, 10), 42); CHECK_EQ_P(e1, v + 5);
    CHECK_EQ_U(_strtoumax_l(v, &e2, 10, loc), 42); CHECK_EQ_P(e2, e1);
    v = "4294967296"; CHECK_EQ_U(strtoumax(v, &e1, 10), 4294967296ULL);
}
TC_CASE(unsigned_minus_one_wraps)
{
    char *e;
    v = "-1"; errno = 0; CHECK_EQ_U(strtoumax(v, &e, 10), UINTMAX_MAX); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, v + 2);
    CHECK_EQ_U(_strtoumax_l(v, &e, 10, loc), UINTMAX_MAX);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    TC_RUN(size_sanity); TC_RUN(limits); TC_RUN(bases); TC_RUN(endptr); TC_RUN(strtoumax_matches); TC_RUN(unsigned_minus_one_wraps);
    _free_locale(loc);
    return tc_end();
}
