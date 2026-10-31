/* TC
 * id:    gwc_wctype_names
 * area:  conv
 * funcs: wctype is_wctype
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  positive
 * conf:  standard
 * ref:   C11 7.30.2.2.2 (wctype; note `_wctype` is a data table export, the function is `wctype`) and 7.30.2.2.1 (iswctype); MS docs iswctype, is_wctype, wctype
 * note:  Only mask-effect assertions are made: handle values are not compared with each other. Runs in the "C" locale.
 */
#include "tc.h"
#include <wchar.h>
#include <wctype.h>
#include <ctype.h>

static const char *volatile vn;
static volatile wchar_t vc;
static unsigned long W(const char *n) { vn = n; return (unsigned long)wctype(vn); }
static int Is(wchar_t c, const char *n) { vc = c; return is_wctype(vc, (wctype_t)W(n)) != 0; }

TC_CASE(names_nonzero)
{
    static const char *const nm[] = { "alnum","alpha","cntrl","digit","graph","lower","print","punct","space","upper","xdigit" };
    size_t i;
    for (i = 0; i < sizeof nm / sizeof nm[0]; i++) { if (W(nm[i]) == 0) tc_fail_strs(__FILE__, __LINE__, "wctype(name)", "nonzero", nm[i], "nonzero"); }
}
TC_CASE(alpha_digit) { CHECK(Is(L'A', "alpha")); CHECK(!Is(L'1', "alpha")); CHECK(Is(L'5', "digit")); CHECK(!Is(L'a', "digit")); }
TC_CASE(xdigit) { CHECK(Is(L'f', "xdigit")); CHECK(Is(L'F', "xdigit")); CHECK(!Is(L'g', "xdigit")); CHECK(Is(L'0', "xdigit")); }
TC_CASE(case_classes) { CHECK(Is(L'a', "lower")); CHECK(!Is(L'a', "upper")); CHECK(Is(L'Z', "upper")); CHECK(!Is(L'Z', "lower")); }
TC_CASE(space_cntrl_punct)
{
    CHECK(Is(L' ', "space")); CHECK(Is(L'\t', "space")); CHECK(!Is(L'x', "space"));
    CHECK(Is(L'\t', "cntrl")); CHECK(!Is(L' ', "cntrl"));
    CHECK(Is(L'!', "punct")); CHECK(!Is(L'a', "punct"));
    CHECK(Is(L'a', "alnum")); CHECK(Is(L'7', "alnum")); CHECK(!Is(L'-', "alnum"));
    CHECK(Is(L'a', "graph")); CHECK(!Is(L' ', "graph")); CHECK(Is(L' ', "print")); CHECK(!Is(L'\n', "print"));
}
TC_CASE(equals_iswctype)
{
    static const char *const nm[] = { "alnum","alpha","cntrl","digit","graph","lower","print","punct","space","upper","xdigit" };
    size_t i; int c;
    for (i = 0; i < sizeof nm / sizeof nm[0]; i++)
        for (c = 0; c < 128; c++) {
            wctype_t t = (wctype_t)W(nm[i]); vc = (wchar_t)c;
            if ((is_wctype(vc, t) != 0) != (iswctype(vc, t) != 0)) tc_fail_strs(__FILE__, __LINE__, "is_wctype", "iswctype", nm[i], "agree");
        }
}
int main(void)
{
    tc_begin();
    TC_RUN(names_nonzero); TC_RUN(alpha_digit); TC_RUN(xdigit); TC_RUN(case_classes); TC_RUN(space_cntrl_punct); TC_RUN(equals_iswctype);
    return tc_end();
}
