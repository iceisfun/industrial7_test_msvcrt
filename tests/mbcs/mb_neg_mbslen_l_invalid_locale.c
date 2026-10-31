/* TC
 * id:    mb_neg_mbslen_l_invalid_locale
 * area:  mbcs
 * funcs:
 * procs: _mbslen_l _mbscmp_l _mbschr_l _mbscpy_s_l _mbsstr_l _create_locale _free_locale
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _mbslen_l, _mbscmp_l, _mbschr_l, _mbsstr_l, _mbscpy_s_l, _create_locale
 * note:  _l variants driven by a real cp932 locale object (the global _setmbcp is never changed). NULL-locale behaviour
 *        is undocumented and lives in mb_neg_probe_mbs_l_null_locale. Error cases use arguments that are invalid regardless
 *        of locale (NULL dst, too-small buffer, not-found searches).
 */
#include "tc.h"
#include <mbstring.h>
#include <errno.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef size_t (__cdecl *slen_t)(const unsigned char *, void *);
typedef int (__cdecl *scmp_t)(const unsigned char *, const unsigned char *, void *);
typedef unsigned char *(__cdecl *schr_t)(const unsigned char *, unsigned int, void *);
typedef unsigned char *(__cdecl *sstr_t)(const unsigned char *, const unsigned char *, void *);
typedef int (__cdecl *scpy_t)(unsigned char *, size_t, const unsigned char *, void *);
static create_t p_create; static free_t p_free; static slen_t p_slen; static scmp_t p_scmp;
static schr_t p_schr; static sstr_t p_sstr; static scpy_t p_scpy;
static void *loc;
static const unsigned char *volatile v_s;
static unsigned char *volatile v_null;
static const unsigned char dbcs[] = {0x82, 0xA0, 0x82, 0xA2, 'a', 0};

#define NEED(p) do { if (!(p)) TC_SKIP("not exported"); if (!loc) TC_SKIP("cp932 locale unavailable"); } while (0)

TC_CASE(valid_locale_object_932_dbcs_counted) { NEED(p_slen); v_s = dbcs; CHECK_EQ_U(p_slen(v_s, loc), 3); }
TC_CASE(scmp_not_equal)
{
    static const unsigned char a[] = {0x82, 0xA0, 0}, b[] = {0x82, 0xA2, 0};
    NEED(p_scmp);
    CHECK(p_scmp(a, b, loc) < 0); CHECK(p_scmp(b, a, loc) > 0); CHECK_EQ_I(p_scmp(a, a, loc), 0);
}
TC_CASE(schr_not_found)
{
    NEED(p_schr); v_s = dbcs;
    CHECK(p_schr(v_s, 0xA082, loc) == NULL);   /* straddles two characters */
    CHECK(p_schr(v_s, 'z', loc) == NULL);
}
TC_CASE(sstr_not_found)
{
    static const unsigned char n[] = {0xA0, 0x82, 0}, n2[] = "zz";
    NEED(p_sstr); v_s = dbcs;
    CHECK(p_sstr(v_s, n, loc) == NULL);
    CHECK(p_sstr(v_s, n2, loc) == NULL);
}
TC_CASE(scpy_s_null_dst_einval)
{
    NEED(p_scpy); tc_iph_count = 0;
    CHECK_EQ_I(p_scpy(v_null, 8, dbcs, loc), EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(scpy_s_small_erange)
{
    unsigned char d[8] = {'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x'};
    NEED(p_scpy); tc_iph_count = 0;
    CHECK_EQ_I(p_scpy(d, 3, dbcs, loc), ERANGE); CHECK_EQ_U(d[0], 0); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_slen = (slen_t)tc_crt_proc("_mbslen_l"); p_scmp = (scmp_t)tc_crt_proc("_mbscmp_l");
    p_schr = (schr_t)tc_crt_proc("_mbschr_l"); p_sstr = (sstr_t)tc_crt_proc("_mbsstr_l");
    p_scpy = (scpy_t)tc_crt_proc("_mbscpy_s_l");
    if (p_create) loc = p_create(0 /*LC_ALL*/, ".932");
    TC_RUN(valid_locale_object_932_dbcs_counted); TC_RUN(scmp_not_equal); TC_RUN(schr_not_found);
    TC_RUN(sstr_not_found); TC_RUN(scpy_s_null_dst_einval); TC_RUN(scpy_s_small_erange);
    if (loc && p_free) p_free(loc);
    return tc_end();
}
