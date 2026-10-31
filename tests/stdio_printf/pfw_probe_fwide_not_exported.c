/* TC
 * id:    pfw_probe_fwide_not_exported
 * area:  stdio_printf
 * funcs: 
 * procs: fwide
 * crt:   all
 * tier:  p2
 * kind:  positive
 * conf:  probe
 * ref:   C11 7.29.3.5 fwide (not provided by MS CRTs)
 * note:  PROBE: fwide is not exported by any MS CRT.
 */
#include "tc.h"

TC_CASE(fwide_absent) { CHECK(tc_crt_proc("fwide") == NULL); }
int main(void) { tc_begin(); TC_RUN(fwide_absent); return tc_end(); }
