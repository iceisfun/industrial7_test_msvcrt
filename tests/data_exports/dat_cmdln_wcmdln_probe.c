/* TC
 * id:    dat_cmdln_wcmdln_probe
 * area:  data_exports
 * kind:  positive
 * funcs: __p__wcmdln
 * procs: 
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100
 * tier:  p1
 * conf:  probe
 * ref:   C runtime internals
 * note:  PROBE: in a narrow main() program is *__p__wcmdln() populated? If so it must be non-empty.
 */
#include "tc.h"

extern wchar_t **__cdecl __p__wcmdln(void);

TC_CASE(wcmdln_ptr_nonnull) { CHECK(__p__wcmdln() != NULL); }
TC_CASE(wcmdln_nonnull_if_populated)
{
    wchar_t **p = __p__wcmdln();
    if (!p) { CHECK(p != NULL); return; }
    CHECK(*p != NULL);
    if (*p) CHECK((*p)[0] != 0);
}
int main(void) { tc_begin(); TC_RUN(wcmdln_ptr_nonnull); TC_RUN(wcmdln_nonnull_if_populated); return tc_end(); }
