/* TC
 * id:    stl_set_error_mode_initial_probe
 * area:  stdlib
 * funcs: _set_error_mode
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _set_error_mode
 * note:  Probes that the initial error mode of a console exe is _OUT_TO_DEFAULT (0).
 */
#include "tc.h"
#include <stdlib.h>

static volatile int v3 = 3;
TC_CASE(initial_default) { CHECK_EQ_I(_set_error_mode(v3), 0); }
int main(void) { tc_begin(); TC_RUN(initial_default); return tc_end(); }
