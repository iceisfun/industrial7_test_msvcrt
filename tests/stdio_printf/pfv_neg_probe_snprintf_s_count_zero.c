/* TC
 * id:    pfv_neg_probe_snprintf_s_count_zero
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p2
 * conf:  probe
 * ref:   MS docs _snprintf_s (count limit, _TRUNCATE), sprintf_s
 * note:  PROBE: _snprintf_s(b,8,0,"abc") has no room for output: expected -1 with b[0]==0 (the handler count is not asserted).
 *        The documented _TRUNCATE / size-1 checks live in pfv_neg_snprintf_s_size1_truncate.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>

static const char *volatile abc = "abc";

TC_CASE(count0_ret) { char b[8]; int r; memset(b, 'Z', 8); tc_iph_count = 0; r = _snprintf_s(b, 8, 0, "%s", abc); CHECK_EQ_I(r, -1); }
TC_CASE(count0_buf0) { char b[8]; memset(b, 'Z', 8); _snprintf_s(b, 8, 0, "%s", abc); CHECK_EQ_I(b[0], 0); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(count0_ret); TC_RUN(count0_buf0);
    return tc_end();
}
