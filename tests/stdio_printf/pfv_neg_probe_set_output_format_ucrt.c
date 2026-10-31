/* TC
 * id:    pfv_neg_probe_set_output_format_ucrt
 * area:  stdio_printf
 * funcs: 
 * procs: _set_output_format
 * crt:   win10
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _set_output_format (obsolete with the UCRT)
 * note:  PROBE: ucrtbase.dll does not export _set_output_format (_get_output_format IS exported by ucrtbase per import lib, so not asserted) (the UCRT always prints two-digit minimum exponents).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
static double D(unsigned long long u) { union { double d; unsigned long long u; } x; x.u = u; return x.d; }
TC_CASE(not_exported) {
    CHECK(tc_crt_proc("_set_output_format") == NULL);
}
int main(void) { tc_begin(); TC_RUN(not_exported); return tc_end(); }
