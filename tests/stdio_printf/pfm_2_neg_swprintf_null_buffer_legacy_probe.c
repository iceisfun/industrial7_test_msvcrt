/* TC
 * id:    pfm_2_neg_swprintf_null_buffer_legacy_probe
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf _snwprintf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs swprintf, _snwprintf, snprintf (UCRT; "if buffer is NULL and count is zero, returns the length")
 * note:  PROBES (each case independent): (1) swprintf(NULL,5,L"x") returns -1, EINVAL, handler called once; (2) swprintf(buf,0,L"x") returns -1
 *        (handler/EINVAL not asserted) and leaves buf untouched; (3) OBSERVED Win10 22H2 19045: legacy _snwprintf(NULL,0,L"abc") returns 3, the needed length (old guess -1 was wrong);
 *        (4) swprintf(buf,5,NULL) returns -1, EINVAL, handler once.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static wchar_t * volatile v_null_buf;
static const wchar_t * volatile v_null_fmt;

TC_CASE(swprintf_null_buffer) { const wchar_t * volatile f = L"x"; int n; tc_iph_count = 0; errno = 0; n = swprintf(v_null_buf, 5, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(swprintf_zero_count) { wchar_t b[8]; const wchar_t * volatile f = L"x"; int n; b[0] = L'Q'; n = swprintf(b, 0, f);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(b[0], L'Q'); }
TC_CASE(snwprintf_count_only) { const wchar_t * volatile f = L"abc"; int n; tc_iph_count = 0; n = _snwprintf(v_null_buf, 0, f);
    CHECK_EQ_I(n, 3); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(swprintf_null_format) { wchar_t b[8]; int n; b[0] = L'Q'; tc_iph_count = 0; errno = 0; n = swprintf(b, 5, v_null_fmt);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(swprintf_null_buffer); TC_RUN(swprintf_zero_count); TC_RUN(snwprintf_count_only); TC_RUN(swprintf_null_format);
    return tc_end();
}
