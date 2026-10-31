/* TC
 * id:    loc_stricoll_l_family
 * area:  locale
 * funcs:
 * procs: _stricoll_l _strnicoll_l _strncoll_l _wcsicoll_l _wcsncoll_l _wcsnicoll_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs strcoll, wcscoll, _strcoll_l, _stricoll_l, _strncoll_l, _strnicoll_l and wide variants
 * note:  Uses the C locale object. mingw import libs lack some of these for old CRTs so all go through procs.
 *        msvcrt.dll/msvcr80 excluded (_create_locale availability not certain).
 */
#include "tc.h"
#include <locale.h>
#include <stddef.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *s2_t)(const char *, const char *, void *);
typedef int (__cdecl *sn_t)(const char *, const char *, size_t, void *);
typedef int (__cdecl *w2_t)(const wchar_t *, const wchar_t *, void *);
typedef int (__cdecl *wn_t)(const wchar_t *, const wchar_t *, size_t, void *);
static create_t p_create; static free_t p_free;
static s2_t p_stricoll; static sn_t p_strncoll, p_strnicoll;
static w2_t p_wcsicoll; static wn_t p_wcsncoll, p_wcsnicoll;
static void *loc;
static const char * volatile va, * volatile vb;
static const wchar_t * volatile wa, * volatile wb;
static volatile size_t vn;
static int sgn(int x) { return x < 0 ? -1 : x > 0; }
#define NEED(p) do { if (!(p) || !loc) TC_SKIP("function not available"); } while (0)

TC_CASE(stricoll_equal_case)
{
    NEED(p_stricoll);
    va = "A"; vb = "a"; CHECK_EQ_I(p_stricoll(va, vb, loc), 0);
    va = "Hello"; vb = "hELLO"; CHECK_EQ_I(p_stricoll(va, vb, loc), 0);
    va = "a"; vb = "b"; CHECK_EQ_I(sgn(p_stricoll(va, vb, loc)), -1);
}
TC_CASE(strncoll_prefix)
{
    NEED(p_strncoll);
    va = "abcX"; vb = "abcY"; vn = 3; CHECK_EQ_I(p_strncoll(va, vb, vn, loc), 0);
    vn = 4; CHECK(p_strncoll(va, vb, vn, loc) != 0);
}
TC_CASE(strnicoll_prefix_case)
{
    NEED(p_strnicoll);
    va = "ABCx"; vb = "abcY"; vn = 3; CHECK_EQ_I(p_strnicoll(va, vb, vn, loc), 0);
    vn = 4; CHECK(p_strnicoll(va, vb, vn, loc) != 0);
}
TC_CASE(wcs_variants)
{
    NEED(p_wcsicoll && p_wcsncoll && p_wcsnicoll);
    wa = L"A"; wb = L"a"; CHECK_EQ_I(p_wcsicoll(wa, wb, loc), 0);
    wa = L"abcX"; wb = L"abcY"; vn = 3; CHECK_EQ_I(p_wcsncoll(wa, wb, vn, loc), 0);
    vn = 4; CHECK(p_wcsncoll(wa, wb, vn, loc) != 0);
    wa = L"ABCx"; wb = L"abcY"; vn = 3; CHECK_EQ_I(p_wcsnicoll(wa, wb, vn, loc), 0);
    vn = 4; CHECK(p_wcsnicoll(wa, wb, vn, loc) != 0);
}
TC_CASE(ordering_sign)
{
    NEED(p_strncoll && p_wcsncoll && p_strnicoll && p_wcsnicoll);
    va = "abcX"; vb = "abcY"; vn = 4; CHECK_EQ_I(sgn(p_strncoll(va, vb, vn, loc)), -1); CHECK_EQ_I(sgn(p_strncoll(vb, va, vn, loc)), 1);
    wa = L"abcX"; wb = L"abcY"; CHECK_EQ_I(sgn(p_wcsncoll(wa, wb, vn, loc)), -1); CHECK_EQ_I(sgn(p_wcsncoll(wb, wa, vn, loc)), 1);
    va = "ABCx"; vb = "abcY"; CHECK_EQ_I(sgn(p_strnicoll(va, vb, vn, loc)), -1);
    wa = L"ABCx"; wb = L"abcY"; CHECK_EQ_I(sgn(p_wcsnicoll(wa, wb, vn, loc)), -1);
}
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_stricoll = (s2_t)tc_crt_proc("_stricoll_l"); p_strncoll = (sn_t)tc_crt_proc("_strncoll_l"); p_strnicoll = (sn_t)tc_crt_proc("_strnicoll_l");
    p_wcsicoll = (w2_t)tc_crt_proc("_wcsicoll_l"); p_wcsncoll = (wn_t)tc_crt_proc("_wcsncoll_l"); p_wcsnicoll = (wn_t)tc_crt_proc("_wcsnicoll_l");
    CHECK(p_create && p_free);
    if (p_create) loc = p_create(LC_ALL, "C");
    TC_RUN(stricoll_equal_case); TC_RUN(strncoll_prefix); TC_RUN(strnicoll_prefix_case); TC_RUN(wcs_variants); TC_RUN(ordering_sign);
    if (p_free && loc) p_free(loc);
    return tc_end();
}
