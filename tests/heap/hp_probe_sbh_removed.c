/* TC
 * id:    hp_probe_sbh_removed
 * area:  heap
 * funcs: malloc free
 * procs: _get_sbh_threshold _set_sbh_threshold
 * crt:   >=100
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_sbh_threshold (removed with the small-block heap in VS2010)
 * note:  PROBING that msvcr100 and later (incl. ucrtbase) export neither _get_sbh_threshold nor _set_sbh_threshold (and that allocation is still fine).
 */
#include "tc.h"
#include <stdlib.h>

TC_CASE(absent)
{
    void *volatile p;
    CHECK(tc_crt_proc("_get_sbh_threshold") == NULL);
    CHECK(tc_crt_proc("_set_sbh_threshold") == NULL);
    p = malloc(64); CHECK(p != NULL); free(p);
}

int main(void) { tc_begin(); TC_RUN(absent); return tc_end(); }
