/* TC
 * id:    pfw_neg_wide_invalid_format_char
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf format specification; UCRT validates conversion characters
 * note:  PROBE: ucrtbase treats an unknown conversion specifier (%y) or a non-conversion char after % (%!) as an invalid format -> invalid parameter handler, returns -1, errno EINVAL. Older CRTs may print the char literally, hence ucrtbase only.
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t * volatile vy = L"a%yb";
static const wchar_t * volatile vbang = L"a%!b";
TC_CASE(control) { wchar_t b[16]; CHECK_EQ_I(_snwprintf(b, 16, L"a%db", 1), 3); }
TC_CASE(pct_y_iph_neg1_EINVAL) { wchar_t b[16]; int n; tc_iph_count = 0; errno = 0; n = _snwprintf(b, 16, vy, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(pct_bang_iph_neg1_EINVAL) { wchar_t b[16]; int n; tc_iph_count = 0; errno = 0; n = _snwprintf(b, 16, vbang, 1); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(control); TC_RUN(pct_y_iph_neg1_EINVAL); TC_RUN(pct_bang_iph_neg1_EINVAL); return tc_end(); }
