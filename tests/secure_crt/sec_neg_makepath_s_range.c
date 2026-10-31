/* TC
 * id:    sec_neg_makepath_s_range
 * area:  secure_crt
 * funcs: _makepath_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _makepath_s (ERANGE when the buffer is too small)
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wchar.h>

static volatile size_t v_short = 9;   /* "C:\d\n.e" is 8 chars, needs 9; in the failing case we give 8 */

TC_CASE(one_byte_short_ERANGE) { char p[32]; int r; r = _makepath_s(p, v_short - 1, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(r, ERANGE); }
TC_CASE(exact_fit_succeeds_control) { char p[32]; int r; r = _makepath_s(p, v_short, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(r, 0); CHECK_STR(p, "C:\\d\\n.e"); }
TC_CASE(errno_ERANGE) { char p[32]; errno = 0; _makepath_s(p, v_short - 1, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(handler_called) { char p[32]; tc_iph_count = 0; _makepath_s(p, v_short - 1, "C:", "\\d\\", "n", ".e"); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(one_byte_short_ERANGE); TC_RUN(exact_fit_succeeds_control); TC_RUN(errno_ERANGE); TC_RUN(handler_called);
    return tc_end();
}
