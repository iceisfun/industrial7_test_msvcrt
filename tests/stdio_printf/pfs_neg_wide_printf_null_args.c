/* TC
 * id:    pfs_neg_wide_printf_null_args
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p0
 * conf:  documented
 * ref:   MS docs Parameter validation; _snprintf, _snwprintf (NULL format/buffer are invalid parameters, errno EINVAL)
 * note:  The NULL %s argument guard lives in pfs_neg_wide_printf_null_string_ok (observed, not documented).
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const wchar_t * volatile nullfmt = NULL;
static wchar_t * volatile nullbuf = NULL;
static const wchar_t * volatile okfmt = L"x";

TC_CASE(null_format)
{
    wchar_t w[8]; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(w, 4, nullfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(null_buffer)
{
    int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0; errno = 0;
    r = _snwprintf(nullbuf, 4, okfmt);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(null_format); TC_RUN(null_buffer); return tc_end(); }
