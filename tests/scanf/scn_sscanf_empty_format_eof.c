/* TC
 * id:    scn_sscanf_empty_format_eof
 * area:  scanf
 * funcs: sscanf
 * crt:   all
 * tier:  p0
 * conf:  probe
 * ref:   C11 7.21.6.2p16 (no conversions, no input failure => 0)
 * note:  Probes sscanf("", "") == 0 (not EOF): empty input AND empty format. C says 0; MSVC implementation may report EOF when input is already at end.
 */
#include "tc.h"
#include <stdio.h>
#include <limits.h>

TC_CASE(both_empty) { int r = sscanf("", ""); CHECK_EQ_I(r, 0); }
int main(void) { tc_begin(); TC_RUN(both_empty); return tc_end(); }
