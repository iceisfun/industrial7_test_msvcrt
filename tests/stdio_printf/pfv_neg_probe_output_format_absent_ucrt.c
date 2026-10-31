/* TC
 * id:    pfv_neg_probe_output_format_absent_ucrt
 * area:  stdio_printf
 * funcs: sprintf
 * procs: _set_output_format
 * crt:   ucrtbase
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_output_format (not available in UCRT)
 * note:  PROBE: ucrtbase does not export _set_output_format (removed with the 2-digit exponent change) and %e of 1.0 stays 1.000000e+00.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

TC_CASE(proc_absent) { CHECK(tc_crt_proc("_set_output_format") == NULL); }
TC_CASE(exp_always_two_digits) { char b[32]; volatile double d = 1.0; sprintf(b, "%e", d); CHECK_STR(b, "1.000000e+00"); }
int main(void) { tc_begin(); TC_RUN(proc_absent); TC_RUN(exp_always_two_digits); return tc_end(); }
