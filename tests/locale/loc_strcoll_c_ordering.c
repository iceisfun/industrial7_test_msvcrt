/* TC
 * id:    loc_strcoll_c_ordering
 * area:  locale
 * funcs: strcoll wcscoll
 * procs: _strcoll_l _wcscoll_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * conf:  standard
 * ref:   C11 7.24.4.3 strcoll (C locale: same as strcmp); MS docs strcoll, wcscoll, _strcoll_l, _wcscoll_l
 * note:  msvcrt.dll and msvcr80 are excluded: _create_locale availability there is not certain.
 */
#include "tc.h"
#include <locale.h>
#include <string.h>
#include <wchar.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *scoll_l_t)(const char *, const char *, void *);
typedef int (__cdecl *wcoll_l_t)(const wchar_t *, const wchar_t *, void *);
static create_t p_create; static free_t p_free; static scoll_l_t p_scoll; static wcoll_l_t p_wcoll;
static const char * volatile va, * volatile vb;
static const wchar_t * volatile wa, * volatile wb;
static int sgn(int x) { return x < 0 ? -1 : x > 0; }
static int sc(const char *a, const char *b) { va = a; vb = b; return sgn(strcoll(va, vb)); }
static int wc(const wchar_t *a, const wchar_t *b) { wa = a; wb = b; return sgn(wcscoll(wa, wb)); }

TC_CASE(lt_gt_eq)
{
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(sc("a", "b"), -1); CHECK_EQ_I(sc("b", "a"), 1); CHECK_EQ_I(sc("abc", "abc"), 0);
    CHECK_EQ_I(sc("ab", "abc"), -1); CHECK_EQ_I(sc("abd", "abc"), 1);
}
TC_CASE(upper_before_lower)
{
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(sc("B", "a"), -1); CHECK_EQ_I(sc("a", "B"), 1); CHECK_EQ_I(sc("A", "a"), -1);
}
TC_CASE(empty_vs_nonempty)
{
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(sc("", "a"), -1); CHECK_EQ_I(sc("a", ""), 1); CHECK_EQ_I(sc("", ""), 0);
}
TC_CASE(wide_equivalents)
{
    setlocale(LC_ALL, "C");
    CHECK_EQ_I(wc(L"a", L"b"), -1); CHECK_EQ_I(wc(L"B", L"a"), -1); CHECK_EQ_I(wc(L"abc", L"abc"), 0);
    CHECK_EQ_I(wc(L"", L"a"), -1); CHECK_EQ_I(wc(L"a", L""), 1);
}
TC_CASE(l_variants)
{
    void *l;
    if (!p_create || !p_free || !p_scoll || !p_wcoll) TC_SKIP("_l functions not exported");
    l = p_create(LC_ALL, "C");
    CHECK(l != NULL);
    if (!l) return;
    va = "a"; vb = "b"; CHECK_EQ_I(sgn(p_scoll(va, vb, l)), -1);
    va = "B"; vb = "a"; CHECK_EQ_I(sgn(p_scoll(va, vb, l)), -1);
    va = "x"; vb = "x"; CHECK_EQ_I(sgn(p_scoll(va, vb, l)), 0);
    va = ""; vb = "a"; CHECK_EQ_I(sgn(p_scoll(va, vb, l)), -1);
    wa = L"B"; wb = L"a"; CHECK_EQ_I(sgn(p_wcoll(wa, wb, l)), -1);
    wa = L"a"; wb = L"a"; CHECK_EQ_I(sgn(p_wcoll(wa, wb, l)), 0);
    p_free(l);
}
int main(void)
{
    tc_begin();
    p_create = (create_t)tc_crt_proc("_create_locale"); p_free = (free_t)tc_crt_proc("_free_locale");
    p_scoll = (scoll_l_t)tc_crt_proc("_strcoll_l"); p_wcoll = (wcoll_l_t)tc_crt_proc("_wcscoll_l");
    TC_RUN(lt_gt_eq); TC_RUN(upper_before_lower); TC_RUN(empty_vs_nonempty); TC_RUN(wide_equivalents); TC_RUN(l_variants);
    return tc_end();
}
