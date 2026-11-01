/* TC
 * id:    pfl_neg_scprintf_l_bad_spec_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscprintf_l _create_locale _free_locale
 * crt:   >=140
 * tier:  p2
 * conf:  observed
 * ref:   MS docs printf format specification syntax
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "bad_spec r == -1 got=3"; zzp_probe2 "_scprintf_l a%yb: r=3 errno=0 iph=0"): _scprintf_l("a%yb") counts
 *        3 characters ("ayb"; the unknown conversion is printed literally, % dropped): no error, no errno, no handler. Old belief wrong.
 *        C11 7.21.6.1p9: undefined behaviour for an invalid conversion, so conforming. Stable.
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
    CHECK_EQ_I(r, 3); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0);
    _free_locale(l);
}

int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(bad_spec);
    return tc_end();
}
