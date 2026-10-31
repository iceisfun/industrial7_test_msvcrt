/* TC
 * id:    pfv_neg_probe_snprintf_s_count_lt_size
 * area:  stdio_printf
 * funcs: _snprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s (count = max characters to store)
 * note:  PROBE: _snprintf_s(b, 16, 3, "%s", "abcdef") with count < sizeOfBuffer: output is cut at count, buffer is "abc",
 *        returns -1 and the invalid-parameter handler is NOT invoked. Not spelled out on the docs page.
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const char *volatile src = "abcdef";
TC_CASE(count_lt_len) { char b[16]; int n; tc_iph_count = 0; n = _snprintf_s(b, 16, 3, "%s", src); CHECK_EQ_I(n, -1); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(count_lt_len); return tc_end(); }
