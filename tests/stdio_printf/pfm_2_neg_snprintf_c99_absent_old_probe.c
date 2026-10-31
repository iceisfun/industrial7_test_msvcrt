/* TC
 * id:    pfm_2_neg_snprintf_c99_absent_old_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: snprintf vsnprintf
 * crt:   msvcr110 msvcr100 msvcr90 msvcr80
 * tier:  p2
 * conf:  probe
 * ref:   MS docs snprintf (C99 snprintf first shipped with VS2013 / msvcr120)
 * note:  PROBES that msvcr80..110 do not export the C99 names snprintf / vsnprintf (only _snprintf / _vsnprintf).
 */
#include "tc.h"

TC_CASE(c99_names_absent)
{
    CHECK(tc_crt_proc("snprintf") == NULL);
}
TC_CASE(c99_v_absent)
{
    CHECK(tc_crt_proc("vsnprintf") == NULL);
}
int main(void) { tc_begin(); TC_RUN(c99_names_absent); TC_RUN(c99_v_absent); return tc_end(); }
