/* TC
 * id:    loc_neg_mbsrtowcs_s_errors
 * area:  locale
 * funcs: setlocale
 * procs: mbsrtowcs_s wcsrtombs_s
 * crt:   >=90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs mbsrtowcs_s, wcsrtombs_s (return value / error conditions)
 * note:  The *src == NULL case is kept out of this test (see loc_neg_wcsrtombs_s_deref_null_probe).
 */
#include "tc.h"
#include <wchar.h>
#include <locale.h>
#include <string.h>
#include <errno.h>

static struct { unsigned long long a, b; } st;
#define PST ((void *)&st)
static void rst(void) { memset(&st, 0, sizeof st); }

#ifndef STRUNCATE
#define STRUNCATE 80
#endif
TC_DECL_PROC(errno_t, mbsrtowcs_s, (size_t *, wchar_t *, size_t, const char **, size_t, void *));
TC_DECL_PROC(errno_t, wcsrtombs_s, (size_t *, char *, size_t, const wchar_t **, size_t, void *));
static const char * volatile v_s = "hello";
static const wchar_t * volatile v_w = L"hello";
#define TRUNC ((size_t)-1)

TC_CASE(mbs_src_null)
{
    wchar_t d[8]; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count; errno = 0;
    e = tc_p_mbsrtowcs_s(&n, d, 8, NULL, 4, PST);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - c0, 1);
}
TC_CASE(mbs_too_small_erange)
{
    wchar_t d[8]; const char *s = v_s; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count; errno = 0;
    e = tc_p_mbsrtowcs_s(&n, d, 3, &s, 5, PST);
    CHECK_EQ_I(e, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count - c0, 1);
}
TC_CASE(mbs_truncate)
{
    wchar_t d[8]; const char *s = v_s; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count;
    e = tc_p_mbsrtowcs_s(&n, d, 3, &s, TRUNC, PST);
    CHECK_EQ_I(e, STRUNCATE); CHECK_WSTR(d, L"he"); CHECK_EQ_I(tc_iph_count - c0, 0);
}
TC_CASE(wcs_src_null)
{
    char d[8]; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count; errno = 0;
    e = tc_p_wcsrtombs_s(&n, d, 8, NULL, 4, PST);
    CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count - c0, 1);
}
TC_CASE(wcs_too_small_erange)
{
    char d[8]; const wchar_t *s = v_w; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count; errno = 0;
    e = tc_p_wcsrtombs_s(&n, d, 3, &s, 5, PST);
    CHECK_EQ_I(e, ERANGE); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count - c0, 1);
}
TC_CASE(wcs_truncate)
{
    char d[8]; const wchar_t *s = v_w; size_t n = 99; errno_t e; int c0; rst();
    if (!tc_iph_install()) TC_SKIP("no iph");
    c0 = tc_iph_count;
    e = tc_p_wcsrtombs_s(&n, d, 3, &s, TRUNC, PST);
    CHECK_EQ_I(e, STRUNCATE); CHECK_STR(d, "he"); CHECK_EQ_I(tc_iph_count - c0, 0);
}
TC_CASE(wcs_unmappable_eilseq)
{
    static const wchar_t bad[] = { 'a', 0x0416, 'b', 0 };
    char d[8]; const wchar_t *s = bad; size_t n = 99; errno_t e;
    if (!tc_iph_install()) TC_SKIP("no iph");
    CHECK(setlocale(LC_ALL, ".1252") != NULL); rst(); errno = 0;
    e = tc_p_wcsrtombs_s(&n, d, 8, &s, 7, PST);
    CHECK_EQ_I(e, EILSEQ); CHECK_EQ_I(errno, EILSEQ);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin(); TC_LOAD_PROC(mbsrtowcs_s); TC_LOAD_PROC(wcsrtombs_s);
    if (!tc_p_mbsrtowcs_s || !tc_p_wcsrtombs_s) { tc_write("TC FAIL " TC_TEST_ID "::exports missing\n"); return 1; }
    TC_RUN(mbs_src_null); TC_RUN(mbs_too_small_erange); TC_RUN(mbs_truncate);
    TC_RUN(wcs_src_null); TC_RUN(wcs_too_small_erange); TC_RUN(wcs_truncate); TC_RUN(wcs_unmappable_eilseq);
    return tc_end();
}
