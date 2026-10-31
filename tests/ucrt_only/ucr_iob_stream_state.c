/* TC
 * id:    ucr_iob_stream_state
 * area:  ucrt_only
 * funcs: __acrt_iob_func fflush ferror
 * crt:   win10
 * tier:  p1
 * conf:  documented
 * ref:   MS docs fflush, ferror
 */
#include "tc.h"
#include <stdio.h>

static volatile unsigned v1 = 1, v2 = 2;

TC_CASE(fflush_stdout) { CHECK_EQ_I(fflush(__acrt_iob_func(v1)), 0); }
TC_CASE(fflush_stderr) { CHECK_EQ_I(fflush(__acrt_iob_func(v2)), 0); }
TC_CASE(no_error) { CHECK_EQ_I(ferror(__acrt_iob_func(v1)), 0); CHECK_EQ_I(ferror(__acrt_iob_func(v2)), 0); }

int main(void) { tc_begin(); TC_RUN(fflush_stdout); TC_RUN(fflush_stderr); TC_RUN(no_error); return tc_end(); }
