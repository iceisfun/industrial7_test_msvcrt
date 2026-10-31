/* TC
 * id:    pfv_neg_sprintf_s_truncate
 * area:  stdio_printf
 * funcs: sprintf_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs sprintf_s: buffer too small -> invalid parameter handler, buffer[0] = 0, returns -1
 * note:  errno ERANGE is checked in its own case (not stated on the sprintf_s page; documented in the Security Features/error table for the _s family).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>
#include <wchar.h>

static const char *volatile src = "abcdef";
TC_CASE(iph_called) { char b[8]; tc_iph_count = 0; sprintf_s(b, 4, "%s", src); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(returns_minus1) { char b[8]; int n = sprintf_s(b, 4, "%s", src); CHECK_EQ_I(n, -1); }
TC_CASE(buf0_zeroed) { char b[8]; int i; for (i = 0; i < 8; i++) b[i] = 'Z'; sprintf_s(b, 4, "%s", src); CHECK_EQ_I(b[0], 0); }
TC_CASE(errno_erange) { char b[8]; errno = 0; sprintf_s(b, 4, "%s", src); CHECK_EQ_I(errno, ERANGE); }
TC_CASE(exact_fit_ok)
{
    char b[8]; int n; tc_iph_count = 0;
    n = sprintf_s(b, 4, "abc");   /* 3 chars + NUL = 4: fits */
    CHECK_EQ_I(n, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
    n = sprintf_s(b, 3, "abc");   /* needs 4 */
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0;
    n = sprintf_s(b, 5, "abc");
    CHECK_EQ_I(n, 3); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(iph_called); TC_RUN(returns_minus1); TC_RUN(buf0_zeroed); TC_RUN(errno_erange); TC_RUN(exact_fit_ok); return tc_end(); }
