/* TC
 * id:    pfl_neg_scprintf_l_bad_spec_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p2
 * conf:  probe
 * ref:   MS docs printf format specification syntax
 * note:  PROBE: ucrt _scprintf_l("a%yb") (unknown conversion) fails like the output functions: -1, EINVAL, invalid parameter handler once.
 */
#include "tc.h"
#include "pfl_shim.h"
#include <errno.h>

static const char * volatile v_f = "a%yb";

TC_CASE(bad_spec)
{
    _locale_t l = _create_locale(LC_ALL, "C"); int r;
    CHECK(l != NULL); if (!l) return;
    tc_iph_count = 0; errno = 0;
    r = pl_scprintf_l(v_f, l, 1);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(bad_spec);
    return tc_end();
}
