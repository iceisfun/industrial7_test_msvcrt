/* TC
 * id:    pff_e_get_output_format_default
 * area:  stdio_printf
 * funcs:
 * procs: _get_output_format sprintf
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _get_output_format
 * note:  ucrtbase behaviour (getter exported, returns 0) is probed separately in pff_neg_set_output_format_absent_ucrt_probe.
 */
#include "tc.h"
#include <stdio.h>

TC_DECL_PROC(unsigned int, _get_output_format, (void));
static volatile double v;

TC_CASE(exported) { CHECK(tc_p__get_output_format != NULL); }
TC_CASE(default_zero) { if (!tc_p__get_output_format) TC_SKIP("not exported"); CHECK_EQ_U(tc_p__get_output_format(), 0); }
int main(void) { tc_begin(); TC_LOAD_PROC(_get_output_format); TC_RUN(exported); TC_RUN(default_zero); return tc_end(); }
