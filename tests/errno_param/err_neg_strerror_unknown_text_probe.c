/* TC
 * id:    err_neg_strerror_unknown_text_probe
 * area:  errno_param
 * kind:  negative
 * funcs: strerror
 * crt:   all
 * tier:  p2
 * conf:  probe
 * ref:   MS docs strerror
 * note:  PROBE: wording of the unknown-errno message ('Unknown error'). Newer UCRT may append the number; only exact 'Unknown error' asserted.
 */
#include "tc.h"
#include <string.h>

static volatile int v_e;

TC_CASE(unknown_9999) { v_e = 9999; CHECK_STR(strerror(v_e), "Unknown error"); }
TC_CASE(unknown_neg1) { v_e = -1;   CHECK_STR(strerror(v_e), "Unknown error"); }

int main(void) { tc_begin(); TC_RUN(unknown_9999); TC_RUN(unknown_neg1); return tc_end(); }
