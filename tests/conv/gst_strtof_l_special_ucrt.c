/* TC
 * id:    gst_strtof_l_special_ucrt
 * area:  conv
 * funcs: _strtof_l _create_locale _free_locale
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   C11 7.22.1.3; MS docs strtof (UCRT supports INF, NAN, hex floats)
 */
#include "tc.h"
#include <stdlib.h>
#include <locale.h>

static _locale_t loc;
static const char *volatile v;

TC_CASE(inf)
{
    char *e;
    v = "inf";      e = NULL; CHECK_EQ_U(tc_fbits(_strtof_l(v, &e, loc)), 0x7f800000u); CHECK_EQ_P(e, v + 3);
    v = "INF";      e = NULL; CHECK_EQ_U(tc_fbits(_strtof_l(v, &e, loc)), 0x7f800000u); CHECK_EQ_P(e, v + 3);
    v = "infinity"; e = NULL; CHECK_EQ_U(tc_fbits(_strtof_l(v, &e, loc)), 0x7f800000u); CHECK_EQ_P(e, v + 8);
    v = "-inf";     e = NULL; CHECK_EQ_U(tc_fbits(_strtof_l(v, &e, loc)), 0xff800000u); CHECK_EQ_P(e, v + 4);
    v = "infx";     e = NULL; CHECK_EQ_U(tc_fbits(_strtof_l(v, &e, loc)), 0x7f800000u); CHECK_EQ_P(e, v + 3);
}
TC_CASE(nan)
{
    char *e; float f;
    v = "nan"; e = NULL; f = _strtof_l(v, &e, loc); CHECK(f != f); CHECK_EQ_P(e, v + 3);
    v = "-nan"; e = NULL; f = _strtof_l(v, &e, loc); CHECK(f != f); CHECK_EQ_P(e, v + 4);
}
TC_CASE(hex)
{
    char *e;
    v = "0x1.8p1";  e = NULL; CHECK_BITS_F(_strtof_l(v, &e, loc), 3.0f); CHECK_EQ_P(e, v + 7);
    v = "0x1p-149"; e = NULL; CHECK_EQ_U(tc_fbits(_strtof_l(v, &e, loc)), 1u); CHECK_EQ_P(e, v + 8);
}
int main(void)
{
    tc_begin();
    loc = _create_locale(LC_ALL, "C");
    if (!loc) { tc_write("TC FAIL " TC_TEST_ID "::__create_locale\n"); return 1; }
    TC_RUN(inf); TC_RUN(nan); TC_RUN(hex);
    _free_locale(loc);
    return tc_end();
}
