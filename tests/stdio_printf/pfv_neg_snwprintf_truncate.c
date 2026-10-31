/* TC
 * id:    pfv_neg_snwprintf_truncate
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snwprintf (count in wide chars; len>count -> negative)
 * note:  Exact fit returns count; whether a NUL is written is probed separately (pfv_probe_snprintf_exact_fit_no_nul).
 */
#include "tc.h"
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#define FILL(b) do { int i_; for (i_ = 0; i_ < 16; i_++) (b)[i_] = L'x'; } while (0)
TC_CASE(ret_minus1) { wchar_t b[16]; volatile int n; FILL(b); n = _snwprintf(b, 4, L"%ls", L"abcdefgh"); CHECK(n < 0); }
TC_CASE(count_wchars_written) { wchar_t b[16]; volatile int n; FILL(b); n = _snwprintf(b, 4, L"%ls", L"abcdefgh"); CHECK(n < 0);
    CHECK(b[0]==L'a' && b[1]==L'b' && b[2]==L'c' && b[3]==L'd'); }
TC_CASE(exact_fit_returns_count) { wchar_t b[16]; volatile int n; FILL(b); n = _snwprintf(b, 4, L"%ls", L"abcd"); CHECK_EQ_I(n, 4);
    CHECK(b[0]==L'a' && b[3]==L'd'); }
TC_CASE(no_write_past_count) { wchar_t b[16]; int i; volatile int n; FILL(b); n = _snwprintf(b, 4, L"%ls", L"abcdefgh"); CHECK(n < 0);
    for (i = 4; i < 16; i++) CHECK(b[i] == L'x'); }

int main(void) { tc_begin(); TC_RUN(ret_minus1); TC_RUN(count_wchars_written); TC_RUN(exact_fit_returns_count); TC_RUN(no_write_past_count); return tc_end(); }
