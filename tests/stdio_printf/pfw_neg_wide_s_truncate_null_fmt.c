/* TC
 * id:    pfw_neg_wide_s_truncate_null_fmt
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf/_snwprintf: -1 when output exceeds count, no NUL; NULL format -> invalid parameter handler, EINVAL, -1
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static void fill(wchar_t *b) { int i; for (i = 0; i < 16; i++) b[i] = L'Z'; }
static const wchar_t * volatile str = L"abcdef";
static const wchar_t * volatile nfmt = NULL;
TC_CASE(s_truncated_returns_minus1) { wchar_t b[16]; int n; fill(b); n = _snwprintf(b, 4, L"%s", str); CHECK_EQ_I(n, -1); CHECK(b[0]==L'a' && b[3]==L'd'); CHECK_EQ_U(b[4], 'Z'); }
TC_CASE(s_exact_fit_no_nul) { wchar_t b[16]; int n; fill(b); n = _snwprintf(b, 6, L"%s", str); CHECK_EQ_I(n, 6); CHECK_EQ_U(b[5], 'f'); CHECK_EQ_U(b[6], 'Z'); }
TC_CASE(null_format) { wchar_t b[16]; int n; fill(b); tc_iph_count = 0; if (!tc_iph_install()) TC_SKIP("no iph"); errno = 0;
    n = _snwprintf(b, 8, nfmt); CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
int main(void) { tc_begin(); TC_RUN(s_truncated_returns_minus1); TC_RUN(s_exact_fit_no_nul); TC_RUN(null_format); return tc_end(); }
