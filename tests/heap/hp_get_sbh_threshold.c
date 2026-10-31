/* TC
 * id:    hp_get_sbh_threshold
 * area:  heap
 * funcs: _get_sbh_threshold
 * crt:   msvcrt msvcr80 msvcr90
 * tier:  p2
 * conf:  documented
 * ref:   MS docs _get_sbh_threshold (returns the current small-block-heap threshold, 0 if SBH not in use; max 1016)
 * note:  msvcr100+ no longer export it (see hp_probe_sbh_removed).
 */
#include "tc.h"

size_t __cdecl _get_sbh_threshold(void);

TC_CASE(range_stable)
{
    size_t a = _get_sbh_threshold(), b = _get_sbh_threshold();
    CHECK(a <= 1016);
    CHECK_EQ_U(a, b);
}

int main(void) { tc_begin(); TC_RUN(range_stable); return tc_end(); }
