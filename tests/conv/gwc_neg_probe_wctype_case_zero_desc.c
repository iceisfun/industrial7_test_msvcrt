/* TC
 * id:    gwc_neg_probe_wctype_case_zero_desc
 * area:  conv
 * funcs: is_wctype
 * procs: wctype
 * crt:   ucrtbase msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.30.2.2.2 (property names are lower case); MS docs wctype, iswctype
 * note:  PROBE: (1) wctype names are matched case-sensitively ("Alpha", "DIGIT" give 0); the standard lists only lower-case names
 *        but extra locale-specific names are permitted, so this is implementation behaviour. (2) is_wctype with descriptor 0
 *        (a bogus wctype result) returns 0 for any character; passing a descriptor not obtained from wctype is not defined by
 *        the standard, but a plain mask test is the believed implementation. Each in its own case.
 */
#include "tc.h"
#include <wchar.h>

typedef unsigned short (__cdecl *wctype_fn)(const char *);
static wctype_fn p_wctype;
static const char *volatile nm;
static volatile wint_t vc;
static volatile unsigned short vd;

static unsigned short W(const char *n) { nm = n; return p_wctype(nm); }

TC_CASE(case_alpha) { if (!p_wctype) TC_SKIP("missing"); CHECK_EQ_U(W("Alpha"), 0); }
TC_CASE(case_digit) { if (!p_wctype) TC_SKIP("missing"); CHECK_EQ_U(W("DIGIT"), 0); }
TC_CASE(zero_desc_alpha) { vc = L'a'; vd = 0; CHECK_EQ_I(is_wctype(vc, vd), 0); }
TC_CASE(zero_desc_digit) { vc = L'5'; vd = 0; CHECK_EQ_I(is_wctype(vc, vd), 0); }
TC_CASE(zero_desc_space) { vc = L' '; vd = 0; CHECK_EQ_I(is_wctype(vc, vd), 0); }
TC_CASE(desc_from_bogus_name)
{
    if (!p_wctype) TC_SKIP("missing");
    vd = W("bogus"); vc = L'a';
    CHECK_EQ_I(is_wctype(vc, vd), 0);
}

int main(void)
{
    tc_begin(); p_wctype = (wctype_fn)tc_crt_proc("wctype");
    TC_RUN(case_alpha); TC_RUN(case_digit); TC_RUN(zero_desc_alpha); TC_RUN(zero_desc_digit);
    TC_RUN(zero_desc_space); TC_RUN(desc_from_bogus_name);
    return tc_end();
}
