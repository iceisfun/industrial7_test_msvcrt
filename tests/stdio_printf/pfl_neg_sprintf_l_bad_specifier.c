/* TC
 * id:    pfl_neg_sprintf_l_bad_specifier
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification syntax; ucrt strict mode: invalid format string -> invalid parameter handler, -1, EINVAL
 * note:  PROBE: ucrt rejects an unknown conversion character after '%' ("%y", "%lq", "%!") via the invalid parameter handler (-1, EINVAL).
 *        Believed from the ucrt state-machine design but not clearly stated in the docs. Edge forms ("%z", trailing lone '%', "%ll") are in pfl_neg_sprintf_l_bad_spec_edge_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_fmt;
static int have_iph;

static void bad(const char *fmt)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !have_iph) TC_SKIP("setup");
    v_fmt = fmt; tc_iph_count = 0; errno = 0;
    r = _sprintf_l(b, (const char *)v_fmt, l, 1, 2);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    _free_locale(l);
}
TC_CASE(pct_y) { bad("a%yb"); }
TC_CASE(pct_lq) { bad("%lq"); }
TC_CASE(pct_bang) { bad("%!"); }
TC_CASE(valid_still_ok)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c);
    if (!l || !have_iph) TC_SKIP("setup");
    tc_iph_count = 0;
    CHECK_EQ_I(_sprintf_l(b, "%d%%", l, 7), 2); CHECK_STR(b, "7%"); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); have_iph = tc_iph_install();
    TC_RUN(pct_y); TC_RUN(pct_lq); TC_RUN(pct_bang); TC_RUN(valid_still_ok);
    return tc_end();
}
