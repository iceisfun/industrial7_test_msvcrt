/* TC
 * id:    gmc_neg_class_null_locale_probe
 * area:  mbcs
 * funcs: _ismbcalnum _ismbcalnum_l _ismbcdigit_l _ismbcpunct_l _setmbcp
 * procs: _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _ismbcalnum_l (locale param)
 * note:  PROBE: a NULL locale argument is accepted silently (uses the current locale) and does not invoke the invalid-parameter handler; _create_locale(LC_ALL,"C") object works too.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <locale.h>

typedef void *(__cdecl *cl_t)(int, const char *);
typedef void (__cdecl *fl_t)(void *);
static volatile unsigned int v;
static int an(unsigned c, _locale_t l) { v = c; return _ismbcalnum_l(v, l) != 0; }
static int di(unsigned c, _locale_t l) { v = c; return _ismbcdigit_l(v, l) != 0; }
static int pu(unsigned c, _locale_t l) { v = c; return _ismbcpunct_l(v, l) != 0; }
TC_CASE(null_locale)
{
    unsigned c;
    CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0;
    for (c = 0; c < 0x80; c++) {
        v = c;
        CHECK_EQ_I(an(c, NULL), _ismbcalnum(v) != 0);
        CHECK_EQ_I(di(c, NULL), (c >= '0' && c <= '9'));
    }
    CHECK(pu('!', NULL)); CHECK(!pu('a', NULL));
    CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(c_locale_object)
{
    cl_t cl = (cl_t)tc_crt_proc("_create_locale");
    fl_t fl = (fl_t)tc_crt_proc("_free_locale");
    _locale_t l;
    if (!cl || !fl) TC_SKIP("_create_locale not exported");
    CHECK_EQ_I(_setmbcp(932), 0); tc_iph_count = 0;
    l = (_locale_t)cl(LC_ALL, "C");
    CHECK(l != NULL);
    if (l) {
        CHECK(an('A', l)); CHECK(an('9', l)); CHECK(!an('!', l));
        CHECK(di('7', l)); CHECK(!di('a', l));
        CHECK(pu('!', l)); CHECK(!pu('A', l));
        fl(l);
    }
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(null_locale); TC_RUN(c_locale_object);
    _setmbcp(-3);
    return tc_end();
}
