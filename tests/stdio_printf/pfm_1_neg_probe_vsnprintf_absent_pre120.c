/* TC
 * id:    pfm_1_neg_probe_vsnprintf_absent_pre120
 * area:  stdio_printf
 * funcs:
 * procs: vsnprintf
 * crt:   msvcr100 msvcr90 msvcr80
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs vsnprintf (C99 vsnprintf shipped with the VS2013 CRT, msvcr120)
 * note:  PROBE: the C99 name `vsnprintf` is not exported by msvcr100 and older (only _vsnprintf). Guess based on VS2013 being
 *        the release that added C99 snprintf-family support; msvcr110 left out as unsure.
 */
#include "tc.h"
#include <stdio.h>

TC_CASE(vsnprintf_not_exported) { CHECK(tc_crt_proc("vsnprintf") == NULL); }

int main(void) { tc_begin(); TC_RUN(vsnprintf_not_exported); return tc_end(); }
