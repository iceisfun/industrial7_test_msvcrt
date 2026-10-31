/* TC
 * id:    ucr_neg_get_stream_buffer_pointers_null_probe
 * area:  ucrt_only
 * kind:  negative
 * funcs:
 * procs: _get_stream_buffer_pointers
 * crt:   win10
 * tier:  p2
 * conf:  probe
 * ref:   ucrtbase export (undocumented)
 * note:  PROBE: _get_stream_buffer_pointers(NULL, ...) does not crash and returns nonzero (EINVAL-style) via the invalid-parameter path.
 *        It is possible the real implementation does no validation and returns 0; the result on real Windows decides.
 */
#include "tc.h"
#include <stdio.h>

typedef int (__cdecl *gsbp_t)(FILE *, char ***, char ***, int **);

TC_CASE(null_stream)
{
    gsbp_t f = (gsbp_t)tc_crt_proc("_get_stream_buffer_pointers");
    char **b = NULL, **p = NULL; int *c = NULL;
    if (!f) TC_SKIP("not exported");
    tc_iph_install();
    CHECK(f(NULL, &b, &p, &c) != 0);
}
int main(void) { tc_begin(); TC_RUN(null_stream); return tc_end(); }
