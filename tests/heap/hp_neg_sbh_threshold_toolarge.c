/* TC
 * id:    hp_neg_sbh_threshold_toolarge
 * area:  heap
 * kind:  negative
 * funcs: _set_sbh_threshold _get_sbh_threshold
 * crt:   msvcr80 msvcr90 msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_sbh_threshold: max 1016 (32-bit); returns 0 on failure
 * note:  PROBING: _set_sbh_threshold(0x7FFF) fails (returns 0) and leaves _get_sbh_threshold unchanged. Docs state the max on x86;
 *        x64 behaviour (SBH absent, probably always 0) is the unknown being measured.
 */
#include "tc.h"
#include <stdlib.h>
#include <malloc.h>

static volatile size_t v_t = 0x7FFF;

TC_CASE(too_large)
{
    size_t before = _get_sbh_threshold(), r;
    r = (size_t)_set_sbh_threshold(v_t);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(_get_sbh_threshold(), before);
}
int main(void) { tc_begin(); TC_RUN(too_large); return tc_end(); }
