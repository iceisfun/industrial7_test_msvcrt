/* TC
 * id:    pfm_1_bare_dot_float_tie_probe
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   all
 * tier:  p0
 * conf:  observed
 * ref:   C11 7.21.6.1 (exact decimal conversion); tie rounding on ucrtbase is selected by the options passed to the worker
 * note:  Observed on Windows 10 22H2 (19045) ucrtbase: the mingw sprintf passes options=2 (no STANDARD_ROUNDING 0x20), so %.f of the exact tie
 *        2.5 gives "3" (legacy half away from zero), same as the older CRTs. With options 0x22 the same ucrtbase gives "2" (reference probe; see
 *        pff_f_round_ties_ucrt). Toolchain/options dependent, not Windows-build dependent, not an ISO C deviation of ucrtbase (class F).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

TC_CASE(tie_2_5) {
    char b[16]; volatile double v = 2.5; sprintf(b, "%.f", v);
    CHECK_STR(b, "3");
}
int main(void) { tc_begin(); TC_RUN(tie_2_5); return tc_end(); }
