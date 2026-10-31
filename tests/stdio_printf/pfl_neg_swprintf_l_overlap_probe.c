/* TC
 * id:    pfl_neg_swprintf_l_overlap_probe
 * area:  stdio_printf
 * funcs: _sprintf_s_l
 * crt:   >=140
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s, _sprintf_s_l ("buffers must not overlap; behavior undefined")
 * note:  PROBE: overlapping source and destination, _sprintf_s_l(buf, size, "%s", loc, buf). Microsoft documents the
 *        result as undefined for the _s variants, so case 1 asserts only "returns without crashing"; case 2 is a pure
 *        probe that exact self-copy (identity, return 3) happens. Kept in its own file for that reason.
 */
#include "tc.h"
#include <stdio.h>
#include <locale.h>

TC_CASE(overlap_no_crash)
{
    char buf[32];
    volatile int n;
    _locale_t loc = _create_locale(LC_ALL, "C");
    CHECK(loc != NULL);
    if (!loc) return;
    buf[0] = 'a'; buf[1] = 'b'; buf[2] = 'c'; buf[3] = 0;
    tc_iph_install();
    n = _sprintf_s_l(buf, sizeof buf, "%s", loc, buf);
    (void)n;
    CHECK(1); /* reached: did not crash or hang */
    _free_locale(loc);
}
TC_CASE(overlap_self_copy_identity_probe)
{
    char buf[32];
    volatile int n;
    _locale_t loc = _create_locale(LC_ALL, "C");
    if (!loc) TC_SKIP("no C locale");
    buf[0] = 'a'; buf[1] = 'b'; buf[2] = 'c'; buf[3] = 0;
    tc_iph_install();
    n = _sprintf_s_l(buf, sizeof buf, "%s", loc, buf);
    (void)n;
    /* probe (most likely value): output is emitted sequentially, so copying a string onto itself is the identity.
       UCRT has no documented overlap check; clearing buf[0] is not expected outside debug builds. */
    CHECK_EQ_I(n, 3);
    CHECK_EQ_I(buf[0], 'a'); CHECK_EQ_I(buf[1], 'b'); CHECK_EQ_I(buf[2], 'c'); CHECK_EQ_I(buf[3], 0);
    _free_locale(loc);
}
int main(void)
{
    tc_begin();
    TC_RUN(overlap_no_crash);
    TC_RUN(overlap_self_copy_identity_probe);
    return tc_end();
}
