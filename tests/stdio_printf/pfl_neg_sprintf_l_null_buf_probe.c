/* TC
 * id:    pfl_neg_sprintf_l_null_buf_probe
 * area:  stdio_printf
 * funcs: _sprintf_l _create_locale _free_locale
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf (parameter validation)
 * note:  PROBE: ucrt _sprintf_l(NULL, "abc", l) (unbounded count, NULL buffer) is rejected: invalid parameter handler once, -1, EINVAL.
 *        Not stated for the unsafe variants in the docs.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <locale.h>

static const char * volatile v_c = "C";
static char * volatile v_nullbuf = NULL;

TC_CASE(null_buf_rejected)
{
    _locale_t l = _create_locale(LC_ALL, v_c); int r;
    if (!l || !tc_iph_install()) TC_SKIP("setup");
    tc_iph_count = 0; errno = 0;
    r = _sprintf_l(v_nullbuf, "abc", l);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL);
    _free_locale(l);
}

int main(void) { tc_begin(); TC_RUN(null_buf_rejected); return tc_end(); }
