/* TC
 * id:    pfm_2_neg_snprintf_absent_msvcrt_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs:
 * procs: snprintf vsnprintf _snprintf _vsnprintf
 * crt:   msvcrt
 * tier:  p2
 * conf:  probe
 * ref:   MS docs snprintf (not documented for the system msvcrt.dll)
 * note:  PROBES that the system msvcrt.dll does not export snprintf / vsnprintf (mingw supplies its own copies), while _snprintf / _vsnprintf exist.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(c99_absent) { CHECK(tc_crt_proc("snprintf") == NULL); CHECK(tc_crt_proc("vsnprintf") == NULL); }
TC_CASE(underscore_present) { CHECK(tc_crt_proc("_snprintf") != NULL); CHECK(tc_crt_proc("_vsnprintf") != NULL); }
int main(void) { tc_begin(); TC_RUN(c99_absent); TC_RUN(underscore_present); return tc_end(); }
