/* TC
 * id:    exp_mbcs_exported
 * area:  exports
 * funcs: 
 * procs: _mbslen _mbscmp _mbsicmp _mbsstr _mbschr _mbsrchr _ismbblead _ismbbtrail _ismbcalpha _mbclen _mbsnbcpy _setmbcp _getmbcp _mbstrlen
 * crt:   all
 * tier:  p1
 * conf:  documented
 * ref:   MS docs MBCS routines; code page 932 (Shift-JIS) lead bytes 0x81-0x9F, 0xE0-0xFC
 * note:  Functional cases switch the MBCS code page to 932 and restore _MB_CP_ANSI (-3) afterwards. _mbscpy/_mbscat are probed in exp_probe_mbscpy_mbscat_export.
 */
#include "tc.h"

typedef size_t (__cdecl *len_t)(const unsigned char *);
typedef int (__cdecl *cmp_t)(const unsigned char *, const unsigned char *);
typedef unsigned char *(__cdecl *str_t)(const unsigned char *, const unsigned char *);
typedef unsigned char *(__cdecl *chr_t)(const unsigned char *, unsigned);
typedef int (__cdecl *is_t)(unsigned);
typedef unsigned char *(__cdecl *ncpy_t)(unsigned char *, const unsigned char *, size_t);
typedef int (__cdecl *setcp_t)(int);
typedef int (__cdecl *getcp_t)(void);
typedef size_t (__cdecl *clen_t)(const unsigned char *);

static int cp932(void)
{
    setcp_t s = (setcp_t)tc_crt_proc("_setmbcp");
    return s && s(932) == 0;
}
static void restore(void) { setcp_t s = (setcp_t)tc_crt_proc("_setmbcp"); if (s) s(-3 /* _MB_CP_ANSI */); }

TC_CASE(length_copy)
{
    len_t ml = (len_t)tc_crt_proc("_mbslen"); len_t ms = (len_t)tc_crt_proc("_mbstrlen"); ncpy_t nc = (ncpy_t)tc_crt_proc("_mbsnbcpy");
    clen_t cl = (clen_t)tc_crt_proc("_mbclen");
    unsigned char d[8];
    CHECK(ml && ms && nc && cl);
    if (!(ml && ms && nc && cl)) return;
    if (!cp932()) { restore(); TC_SKIP("code page 932 unavailable"); }
    CHECK_EQ_U(ml((const unsigned char *)"ab\x82\xa0" "c"), 4);   /* a b <DBCS> c */
    CHECK_EQ_U(cl((const unsigned char *)"\x82\xa0"), 2);
    CHECK_EQ_U(cl((const unsigned char *)"x"), 1);
    d[0] = d[1] = d[2] = d[3] = 'Q'; d[4] = 0;
    nc(d, (const unsigned char *)"ab", 2);
    CHECK_EQ_I(d[0], 'a'); CHECK_EQ_I(d[1], 'b');
    restore();
    CHECK_EQ_U(ms((const unsigned char *)"abc"), 3);   /* locale based; "C" locale: bytes */
}
TC_CASE(compare_search)
{
    cmp_t c = (cmp_t)tc_crt_proc("_mbscmp"), ci = (cmp_t)tc_crt_proc("_mbsicmp");
    str_t st = (str_t)tc_crt_proc("_mbsstr"); chr_t ch = (chr_t)tc_crt_proc("_mbschr"), rc = (chr_t)tc_crt_proc("_mbsrchr");
    const unsigned char *s = (const unsigned char *)"abcabc";
    CHECK(c && ci && st && ch && rc);
    if (!(c && ci && st && ch && rc)) return;
    CHECK_EQ_I(c((const unsigned char *)"abc", (const unsigned char *)"abc"), 0);
    CHECK(c((const unsigned char *)"abc", (const unsigned char *)"abd") < 0);
    CHECK_EQ_I(ci((const unsigned char *)"ABC", (const unsigned char *)"abc"), 0);
    CHECK(st(s, (const unsigned char *)"ca") == s + 2);
    CHECK(ch(s, 'b') == s + 1);
    CHECK(rc(s, 'b') == s + 4);
    CHECK(ch(s, 'z') == NULL);
}
TC_CASE(lead_trail)
{
    is_t lead = (is_t)tc_crt_proc("_ismbblead"), trail = (is_t)tc_crt_proc("_ismbbtrail"), alpha = (is_t)tc_crt_proc("_ismbcalpha");
    CHECK(lead && trail && alpha);
    if (!(lead && trail && alpha)) return;
    if (!cp932()) { restore(); TC_SKIP("code page 932 unavailable"); }
    CHECK(lead(0x81) != 0); CHECK(lead(0x9F) != 0); CHECK(lead(0xE0) != 0);
    CHECK_EQ_I(lead(0x41), 0); CHECK_EQ_I(lead(0xA0), 0);
    CHECK(trail(0x40) != 0); CHECK(trail(0xFC) != 0);
    CHECK_EQ_I(trail(0x20), 0);
    CHECK(alpha('a') != 0);
    CHECK_EQ_I(alpha('1'), 0);
    restore();
}
TC_CASE(codepage)
{
    setcp_t s = (setcp_t)tc_crt_proc("_setmbcp"); getcp_t g = (getcp_t)tc_crt_proc("_getmbcp");
    CHECK(s && g);
    if (!(s && g)) return;
    if (s(932) != 0) { restore(); TC_SKIP("code page 932 unavailable"); }
    CHECK_EQ_I(g(), 932);
    restore();
}
int main(void)
{
    tc_begin();
    TC_RUN(length_copy); TC_RUN(compare_search); TC_RUN(lead_trail); TC_RUN(codepage);
    restore();
    return tc_end();
}
