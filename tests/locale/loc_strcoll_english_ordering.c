/* TC
 * id:    loc_strcoll_english_ordering
 * area:  locale
 * funcs: strcoll setlocale
 * procs: _strcoll_l _create_locale _free_locale
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  observed
 * ref:   MS docs strcoll (uses the LC_COLLATE locale; NLS collation); CompareString linguistic sort
 * note:  Windows NLS sort: alphabetical not ASCII; at tertiary level lowercase sorts before uppercase of the same
 *        letter; accented letters sort after the plain letter when otherwise equal. Hyphen/word-sort handling is NOT asserted.
 */
#include "tc.h"
#include <locale.h>
#include <string.h>

typedef void *(__cdecl *create_t)(int, const char *);
typedef void (__cdecl *free_t)(void *);
typedef int (__cdecl *scoll_l_t)(const char *, const char *, void *);
static const char * volatile va, * volatile vb;
static int sgn(int x) { return x < 0 ? -1 : x > 0; }
static int sc(const char *a, const char *b) { va = a; vb = b; return sgn(strcoll(va, vb)); }
#define EN "English_United States.1252"
static const char * volatile v_en = EN;
static int en(void) { return setlocale(LC_ALL, v_en) != NULL; }

TC_CASE(alpha_vs_ascii_order)
{
    if (!en()) TC_SKIP("no English locale");
    CHECK_EQ_I(sc("a", "B"), -1);       /* ASCII order would give 'B' < 'a' */
    CHECK_EQ_I(sc("B", "a"), 1);
    CHECK_EQ_I(sc("apple", "Banana"), -1);
    setlocale(LC_ALL, "C");
}
TC_CASE(lower_before_upper)
{
    if (!en()) TC_SKIP("no English locale");
    CHECK(sc("a", "A") != 0);
    CHECK_EQ_I(sc("a", "A"), -1);
    CHECK_EQ_I(sc("A", "a"), 1);
    setlocale(LC_ALL, "C");
}
TC_CASE(accent_sorts_after_plain)
{
    if (!en()) TC_SKIP("no English locale");
    CHECK_EQ_I(sc("resume", "r\xE9sum\xE9"), -1);
    CHECK_EQ_I(sc("r\xE9sum\xE9", "resume"), 1);
    /* primary difference dominates the accent */
    CHECK_EQ_I(sc("r\xE9sum\xE9", "resumf"), -1);
    setlocale(LC_ALL, "C");
}
TC_CASE(l_variant_matches_global)
{
    create_t cr = (create_t)tc_crt_proc("_create_locale"); free_t fr = (free_t)tc_crt_proc("_free_locale");
    scoll_l_t sl = (scoll_l_t)tc_crt_proc("_strcoll_l"); void *l;
    if (!cr || !fr || !sl) TC_SKIP("_l functions not exported");
    l = cr(LC_ALL, EN);
    if (!l) TC_SKIP("no English locale");
    setlocale(LC_ALL, "C");   /* global is C, locale object is English: must differ from global */
    va = "a"; vb = "B"; CHECK_EQ_I(sgn(sl(va, vb, l)), -1);
    CHECK_EQ_I(sc("a", "B"), 1);
    va = "a"; vb = "A"; CHECK_EQ_I(sgn(sl(va, vb, l)), -1);
    fr(l);
}
int main(void)
{
    tc_begin();
    TC_RUN(alpha_vs_ascii_order); TC_RUN(lower_before_upper); TC_RUN(accent_sorts_after_plain); TC_RUN(l_variant_matches_global);
    setlocale(LC_ALL, "C");
    return tc_end();
}
