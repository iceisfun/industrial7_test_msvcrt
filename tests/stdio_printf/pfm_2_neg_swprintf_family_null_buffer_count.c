/* TC
 * id:    pfm_2_neg_swprintf_family_null_buffer_count
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf_s _snwprintf_s
 * crt:   >=140
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_s, swprintf_s, _snprintf_s, _snwprintf_s ("If buffer or format is a null pointer, or sizeOfBuffer is zero, the invalid parameter handler is invoked ... -1 returned, errno set to EINVAL"); Parameter Validation
 * note:  Secure wide variants only; the non-_s swprintf / _snwprintf NULL-buffer behaviour is in pfm_2_neg_swprintf_null_buffer_legacy_probe.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static wchar_t * volatile v_null_buf;
static const wchar_t * volatile v_null_fmt;

#define EXPECT_EINVAL(call) do { int tc_n; tc_iph_count = 0; errno = 0; tc_n = (call); \
    CHECK_EQ_I(tc_n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); } while (0)

TC_CASE(swprintf_s_null_buffer)  { const wchar_t * volatile f = L"x"; EXPECT_EINVAL(swprintf_s(v_null_buf, 5, f)); }
TC_CASE(swprintf_s_zero_size)    { wchar_t b[8] = {L'Q'}; const wchar_t * volatile f = L"x"; EXPECT_EINVAL(swprintf_s(b, 0, f)); CHECK_EQ_I(b[0], L'Q'); }
TC_CASE(swprintf_s_null_format)  { wchar_t b[8]; b[0] = L'Q'; EXPECT_EINVAL(swprintf_s(b, 8, v_null_fmt)); }
TC_CASE(snwprintf_s_null_buffer) { const wchar_t * volatile f = L"x"; EXPECT_EINVAL(_snwprintf_s(v_null_buf, 5, 5, f)); }
TC_CASE(snwprintf_s_zero_size)   { wchar_t b[8] = {L'Q'}; const wchar_t * volatile f = L"x"; EXPECT_EINVAL(_snwprintf_s(b, 0, 5, f)); CHECK_EQ_I(b[0], L'Q'); }
TC_CASE(snwprintf_s_null_format) { wchar_t b[8]; b[0] = L'Q'; EXPECT_EINVAL(_snwprintf_s(b, 8, 4, v_null_fmt)); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(swprintf_s_null_buffer); TC_RUN(swprintf_s_zero_size); TC_RUN(swprintf_s_null_format);
    TC_RUN(snwprintf_s_null_buffer); TC_RUN(snwprintf_s_zero_size); TC_RUN(snwprintf_s_null_format);
    return tc_end();
}
