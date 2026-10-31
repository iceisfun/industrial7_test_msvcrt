/* TC
 * id:    loc_neg_probe_strftime_l_null_locale
 * area:  locale
 * funcs: _strftime_l _wcsftime_l
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strftime, _strftime_l (parameter validation: NULL tm / NULL buffer -> invalid parameter handler, errno EINVAL)
 * note:  PROBE: NULL _locale_t is believed to mean the current locale ("%Y" -> "2024"); NULL tm and NULL buffer (count>0) are
 *        believed to return 0, set errno EINVAL and invoke the invalid-parameter handler. Wide variants alike.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>
#include <locale.h>

static _locale_t volatile v_nl;
static struct tm *volatile v_nulltm;
static char *volatile v_nullb;
static wchar_t *volatile v_nullw;

static struct tm mk(void)
{
    struct tm t; char *p = (char *)&t; size_t i; for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 124; t.tm_mon = 0; t.tm_mday = 15; t.tm_wday = 1; t.tm_yday = 14; return t;
}
TC_CASE(null_locale_year)
{
    struct tm t = mk(); char b[64]; wchar_t w[64]; setlocale(LC_ALL, "C");
    CHECK_EQ_U(_strftime_l(b, 64, "%Y", &t, v_nl), 4); CHECK_STR(b, "2024");
    CHECK_EQ_U(_wcsftime_l(w, 64, L"%Y", &t, v_nl), 4); CHECK_WSTR(w, L"2024");
}
TC_CASE(null_tm)
{
    char b[64]; wchar_t w[64]; if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_U(_strftime_l(b, 64, "%Y", v_nulltm, v_nl), 0); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    errno = 0;
    CHECK_EQ_U(_wcsftime_l(w, 64, L"%Y", v_nulltm, v_nl), 0); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 2);
}
TC_CASE(null_buffer)
{
    struct tm t = mk(); if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0; errno = 0;
    CHECK_EQ_U(_strftime_l(v_nullb, 64, "%Y", &t, v_nl), 0); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
    errno = 0;
    CHECK_EQ_U(_wcsftime_l(v_nullw, 64, L"%Y", &t, v_nl), 0); CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 2);
}
int main(void)
{
    tc_begin();
    TC_RUN(null_locale_year); TC_RUN(null_tm); TC_RUN(null_buffer);
    return tc_end();
}
