/* TC
 * id:    loc_strxfrm_orders_like_strcoll
 * area:  locale
 * funcs: strxfrm strcoll wcsxfrm wcscoll setlocale
 * crt:   all
 * tier:  p1
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.24.4.5
 * note:  strcmp on transformed strings has the same sign as strcoll. Transformed length is not asserted. Skips if the 1252 locale is unavailable.
 */
#include "tc.h"
#include <string.h>
#include <wchar.h>
#include <locale.h>

static int sgn(int v) { return v < 0 ? -1 : v > 0; }
static int enter(void) { volatile const char *n = "English_United States.1252"; return setlocale(LC_ALL, (const char *)n) != NULL; }

static void pair(const char *a, const char *b)
{
    char xa[256], xb[256]; size_t na, nb; int c;
    c = strcoll(a, b);
    na = strxfrm(xa, a, sizeof xa); nb = strxfrm(xb, b, sizeof xb);
    CHECK(na < sizeof xa); CHECK(nb < sizeof xb);
    CHECK_EQ_I(sgn(strcmp(xa, xb)), sgn(c));
}
TC_CASE(pair_a_B) { if (!enter()) TC_SKIP("locale unavailable"); pair("a", "B"); pair("B", "a"); setlocale(LC_ALL, "C"); }
TC_CASE(pair_resume) { if (!enter()) TC_SKIP("locale unavailable"); pair("resume", "r\xE9sum\xE9"); pair("r\xE9sum\xE9", "resume"); setlocale(LC_ALL, "C"); }
TC_CASE(pair_abc_abd) { if (!enter()) TC_SKIP("locale unavailable"); pair("abc", "abd"); pair("abd", "abc"); pair("abc", "abc"); setlocale(LC_ALL, "C"); }
TC_CASE(wide_pair)
{
    wchar_t xa[256], xb[256]; int c;
    volatile const wchar_t *a = L"a", *b = L"B";
    if (!enter()) TC_SKIP("locale unavailable");
    c = wcscoll((const wchar_t *)a, (const wchar_t *)b);
    CHECK(wcsxfrm(xa, (const wchar_t *)a, 256) < 256);
    CHECK(wcsxfrm(xb, (const wchar_t *)b, 256) < 256);
    CHECK_EQ_I(sgn(wcscmp(xa, xb)), sgn(c));
    setlocale(LC_ALL, "C");
}
TC_CASE(length_query_ge_strlen_not_asserted)
{
    size_t n;
    if (!enter()) TC_SKIP("locale unavailable");
    n = strxfrm(NULL, "abc", 0);
    CHECK(n > 0);
    setlocale(LC_ALL, "C");
}
int main(void)
{
    tc_begin();
    TC_RUN(pair_a_B); TC_RUN(pair_resume); TC_RUN(pair_abc_abd); TC_RUN(wide_pair); TC_RUN(length_query_ge_strlen_not_asserted);
    return tc_end();
}
