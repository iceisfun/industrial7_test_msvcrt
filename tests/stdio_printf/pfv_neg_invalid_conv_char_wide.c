/* TC
 * id:    pfv_neg_invalid_conv_char_wide
 * area:  stdio_printf
 * kind:  negative
 * funcs: swprintf _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs "Parameter validation", format specification syntax: an unknown conversion character is an invalid format -> handler, -1, EINVAL
 * note:  Wide twin of pfv_neg_invalid_conv_char (narrow). UCRT only; pre-UCRT behaviour is a separate probe. wprintf is not called
 *        (it would write to stdout); the same wide parser serves it.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static int ret, iph, err;
static void run_sw(const wchar_t *f)
{
    wchar_t b[64]; tc_iph_count = 0; errno = 0;
    ret = swprintf(b, 64, f); iph = tc_iph_count; err = errno;
}
static void run_sn(const wchar_t *f)
{
    wchar_t b[64]; tc_iph_count = 0; errno = 0;
    ret = _snwprintf(b, 64, f); iph = tc_iph_count; err = errno;
}
static const wchar_t *volatile f_y = L"%y";
static const wchar_t *volatile f_q = L"ab%-5.2q";
static const wchar_t *volatile f_q2 = L"%q";

TC_CASE(swprintf_pct_y) { run_sw(f_y); CHECK_EQ_I(ret, -1); }
TC_CASE(swprintf_pct_q_after_flags) { run_sw(f_q); CHECK_EQ_I(ret, -1); }
TC_CASE(snwprintf_pct_q) { run_sn(f_q2); CHECK_EQ_I(ret, -1); CHECK_EQ_I(err, EINVAL); CHECK_EQ_I(iph, 1); }
TC_CASE(errno_einval) { run_sw(f_y); CHECK_EQ_I(err, EINVAL); }
TC_CASE(iph_count) { run_sw(f_y); CHECK_EQ_I(iph, 1); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(swprintf_pct_y); TC_RUN(swprintf_pct_q_after_flags); TC_RUN(snwprintf_pct_q); TC_RUN(errno_einval); TC_RUN(iph_count);
    return tc_end();
}
