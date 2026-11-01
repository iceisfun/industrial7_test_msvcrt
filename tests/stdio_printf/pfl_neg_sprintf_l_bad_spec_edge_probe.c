/* TC
 * id:    pfl_neg_sprintf_l_bad_spec_edge_probe
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification syntax
 * note:  Observed on Windows 10 22H2: the non-secure _sprintf_l does NOT validate incomplete conversion specifications: "%z" -> 0, "abc%" -> 3
 *        ("abc"), "%ll" -> 0; no handler call, errno untouched (0). Only the _s/_p families reject them.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_fmt;
static int have_iph;

static void bad(const char *fmt, int want)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    v_fmt = fmt; tc_iph_count = 0; errno = 0;
    r = _sprintf_l(b, (const char *)v_fmt, l, 1, 2);
    CHECK_EQ_I(r, want); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(errno, 0);
    _free_locale(l);
}
TC_CASE(pct_z) { bad("%z", 0); }
TC_CASE(pct_trailing_lone) { bad("abc%", 3); }
TC_CASE(pct_ll_without_conv) { bad("%ll", 0); }

int main(void) { tc_begin(); have_iph = tc_iph_install(); TC_RUN(pct_z); TC_RUN(pct_trailing_lone); TC_RUN(pct_ll_without_conv); return tc_end(); }
