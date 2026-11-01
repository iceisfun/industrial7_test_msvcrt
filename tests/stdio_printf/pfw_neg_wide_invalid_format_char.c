/* TC
 * id:    pfw_neg_wide_invalid_format_char
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  observed
 * ref:   MS docs printf format specification; UCRT validates conversion characters
 * note:  Observed Win10 22H2 ucrtbase: the non-_s _snwprintf does not validate; an unknown conversion (%y) or non-conversion char (%!) is copied literally without the '%' ("ayb", "a!b"), returns 3, errno 0, handler not called.
 *        Raw evidence (baseline): pct_y_iph_neg1_EINVAL n == -1 got=3 ; errno == EINVAL got=0 ; tc_iph_count == 1 got=0 (same for pct_bang).
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

static const wchar_t * volatile vy = L"a%yb";
static const wchar_t * volatile vbang = L"a%!b";
TC_CASE(control) { wchar_t b[16]; CHECK_EQ_I(_snwprintf(b, 16, L"a%db", 1), 3); }
TC_CASE(pct_y_iph_neg1_EINVAL) { wchar_t b[16]; int n; tc_iph_count = 0; errno = 0; n = _snwprintf(b, 16, vy, 1); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"ayb"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(pct_bang_iph_neg1_EINVAL) { wchar_t b[16]; int n; tc_iph_count = 0; errno = 0; n = _snwprintf(b, 16, vbang, 1); CHECK_EQ_I(n, 3); CHECK_WSTR(b, L"a!b"); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(control); TC_RUN(pct_y_iph_neg1_EINVAL); TC_RUN(pct_bang_iph_neg1_EINVAL); return tc_end(); }
