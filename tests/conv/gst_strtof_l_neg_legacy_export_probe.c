/* TC
 * id:    gst_strtof_l_neg_legacy_export_probe
 * area:  conv
 * funcs:
 * procs: _strtof_l _wcstof_l strtof
 * crt:   msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs strtof/_strtof_l, wcstof/_wcstof_l requirements (VS2013 is the documented introduction of strtof/wcstof)
 * note:  Probes whether _strtof_l and _wcstof_l are exported by pre-msvcr120 CRTs at all. Asserted guess: exported (mingw import libraries list them), though the docs only guarantee strtof from msvcr120. Result tells us which legacy tests may be meaningful.
 */
#include "tc.h"

TC_CASE(strtof_l_exported)  { CHECK(tc_crt_proc("_strtof_l") != NULL); }
TC_CASE(wcstof_l_exported)  { CHECK(tc_crt_proc("_wcstof_l") != NULL); }
TC_CASE(strtof_absent)      { CHECK(tc_crt_proc("strtof") == NULL); }
int main(void) { tc_begin(); TC_RUN(strtof_l_exported); TC_RUN(wcstof_l_exported); TC_RUN(strtof_absent); return tc_end(); }
