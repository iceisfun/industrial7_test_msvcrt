/* TC
 * id:    gwc_neg_wctype_unknown_names
 * area:  conv
 * funcs:
 * procs: wctype
 * crt:   ucrtbase msvcr120
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.30.2.2.2 (wctype returns zero if the property is not valid); MS docs wctype, iswctype
 * note:  The CRT function is `wctype` (`_wctype` is the ctype table data export, not callable). Complements ct_neg_wctype_unknown_name
 *        with leading/trailing space and trailing newline. Case-sensitivity and zero-descriptor behaviour are NOT asserted here
 *        (locale-defined names / undefined descriptor): see gwc_neg_probe_wctype_case_zero_desc.
 */
#include "tc.h"
#include <wchar.h>

typedef unsigned short (__cdecl *wctype_fn)(const char *);
static wctype_fn p_wctype;
static const char *volatile nm;

static unsigned short W(const char *n) { nm = n; return p_wctype(nm); }

TC_CASE(exported) { CHECK(p_wctype != NULL); }
TC_CASE(empty_and_bogus)
{
    if (!p_wctype) TC_SKIP("missing");
    CHECK_EQ_U(W(""), 0); CHECK_EQ_U(W("bogus"), 0); CHECK_EQ_U(W("alph"), 0);
}
TC_CASE(trailing_space_and_junk)
{
    if (!p_wctype) TC_SKIP("missing");
    CHECK_EQ_U(W("alpha "), 0); CHECK_EQ_U(W(" alpha"), 0); CHECK_EQ_U(W("alpha\n"), 0);
}
TC_CASE(sanity_valid_name_nonzero) { if (!p_wctype) TC_SKIP("missing"); CHECK(W("alpha") != 0); }
int main(void)
{
    tc_begin(); p_wctype = (wctype_fn)tc_crt_proc("wctype");
    TC_RUN(exported); TC_RUN(empty_and_bogus); TC_RUN(trailing_space_and_junk); TC_RUN(sanity_valid_name_nonzero);
   
    return tc_end();
}
