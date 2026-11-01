/* TC
 * id:    pfw_neg_wide_invalid_format_trailing_pct
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification: incomplete conversion spec is an invalid format
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): a format ending in a lone '%' (or '%' + flags with no conversion) is NOT rejected: _snwprintf returns 3 with "abc", errno untouched,
 *        no handler call (old guess -1/EINVAL/handler was wrong).
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t * volatile vend = L"abc%";
static const wchar_t * volatile vflags = L"abc%-5";
TC_CASE(control) { wchar_t b[16]; CHECK_EQ_I(_snwprintf(b, 16, L"abc%%"), 4); }
TC_CASE(trailing_lone_pct) { wchar_t b[16]; int n; tc_iph_count = 0; errno = 0; n = _snwprintf(b, 16, vend); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(trailing_pct_flags_no_conv) { wchar_t b[16]; int n; tc_iph_count = 0; errno = 0; n = _snwprintf(b, 16, vflags); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"abc"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(control); TC_RUN(trailing_lone_pct); TC_RUN(trailing_pct_flags_no_conv); return tc_end(); }
