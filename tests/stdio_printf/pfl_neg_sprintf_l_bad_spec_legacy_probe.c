/* TC
 * id:    pfl_neg_sprintf_l_bad_spec_legacy_probe
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   msvcr90 msvcr100 msvcr110 msvcr120
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification syntax
 * note:  PROBE: VS2005+ era CRTs (msvcr90..120) also treat an unknown conversion character ("%y") as an invalid parameter
 *        (handler called, -1, EINVAL) rather than printing it leniently.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static const char * volatile v_c = "C";
static volatile const char *v_fmt = "a%yb";

TC_CASE(pct_y_rejected)
{
    char b[32]; _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !tc_iph_install()) TC_SKIP("setup");
    tc_iph_count = 0; errno = 0;
    r = _sprintf_l(b, (const char *)v_fmt, l, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    _free_locale(l);
}

int main(void) { tc_begin(); TC_RUN(pct_y_rejected); return tc_end(); }
