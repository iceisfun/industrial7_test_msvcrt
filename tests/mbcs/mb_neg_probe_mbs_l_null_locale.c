/* TC
 * id:    mb_neg_probe_mbs_l_null_locale
 * area:  mbcs
 * funcs: _setmbcp _getmbcp
 * procs: _mbslen_l _mbscmp_l _mbschr_l _mbscpy_s_l _mbsstr_l
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   none (documentation does not say what a NULL _locale_t means for the _mbs*_l functions)
 * note:  PROBE: guess that a NULL locale means "use the current locale / current multibyte code page" (as in UCRT's
 *        _LocaleUpdate), i.e. results equal the non-_l functions at cp932 and the invalid-parameter handler is NOT invoked.
 *        The alternative (NULL rejected with EINVAL + handler) is what the original spec guessed.
 */
#include "tc.h"
#include <mbstring.h>
#include <mbctype.h>
#include <errno.h>

typedef size_t (__cdecl *slen_t)(const unsigned char *, void *);
typedef int (__cdecl *scmp_t)(const unsigned char *, const unsigned char *, void *);
typedef unsigned char *(__cdecl *schr_t)(const unsigned char *, unsigned int, void *);
typedef unsigned char *(__cdecl *sstr_t)(const unsigned char *, const unsigned char *, void *);
typedef int (__cdecl *scpy_t)(unsigned char *, size_t, const unsigned char *, void *);
static slen_t p_slen; static scmp_t p_scmp; static schr_t p_schr; static sstr_t p_sstr; static scpy_t p_scpy;
static int old_cp;
static const unsigned char dbcs[] = {0x82, 0xA0, 0x82, 0xA2, 'a', 0};

TC_CASE(slen_null_locale)
{
    if (!p_slen) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_U(p_slen(dbcs, NULL), 3); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(scmp_null_locale)
{
    static const unsigned char a[] = {0x82, 0xA0, 0};
    if (!p_scmp) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_scmp(a, a, NULL), 0); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(schr_null_locale)
{
    if (!p_schr) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK(p_schr(dbcs, 'a', NULL) == dbcs + 4); CHECK_EQ_I(tc_iph_count, 0);
}
TC_CASE(scpy_s_null_locale)
{
    unsigned char d[8];
    if (!p_scpy) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK_EQ_I(p_scpy(d, sizeof d, dbcs, NULL), 0); CHECK_EQ_I(tc_iph_count, 0);
    CHECK_EQ_U(d[0], 0x82); CHECK_EQ_U(d[4], 'a'); CHECK_EQ_U(d[5], 0);
}
TC_CASE(sstr_null_locale)
{
    static const unsigned char n[] = {0x82, 0xA2, 0};
    if (!p_sstr) TC_SKIP("not exported");
    tc_iph_count = 0;
    CHECK(p_sstr(dbcs, n, NULL) == dbcs + 2); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_slen = (slen_t)tc_crt_proc("_mbslen_l"); p_scmp = (scmp_t)tc_crt_proc("_mbscmp_l");
    p_schr = (schr_t)tc_crt_proc("_mbschr_l"); p_sstr = (sstr_t)tc_crt_proc("_mbsstr_l");
    p_scpy = (scpy_t)tc_crt_proc("_mbscpy_s_l");
    old_cp = _getmbcp(); _setmbcp(932);
    TC_RUN(slen_null_locale); TC_RUN(scmp_null_locale); TC_RUN(schr_null_locale);
    TC_RUN(scpy_s_null_locale); TC_RUN(sstr_null_locale);
    _setmbcp(old_cp);
    return tc_end();
}
