/* TC
 * id:    sec_neg_makepath_s_empty_probe
 * area:  secure_crt
 * funcs: _makepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _makepath_s
 * note:  PROBE: after ERANGE the output buffer's first byte is believed to be set to 0 (docs do not state it for _makepath_s).
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile size_t v_short = 8;

TC_CASE(path_first_byte_zero) { char p[32]; int r; memset(p, 'x', sizeof p); r = _makepath_s(p, v_short, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(r, ERANGE); CHECK_EQ_I(p[0], 0); }
int main(void) { tc_begin(); tc_iph_install(); TC_RUN(path_first_byte_zero); return tc_end(); }
