/* TC
 * id:    pfs_wstr_neg_snwprintf_truncate_s
 * area:  stdio_printf
 * funcs: _snwprintf
 * crt:   all
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snwprintf: count reached -> returns -1, no NUL appended
 * note:  kind negative: output truncated
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t *volatile w6 = L"abcdef";
static const char *volatile n6 = "abcdef";

TC_CASE(truncated_s) {
    wchar_t w[8]; int n, i; for (i = 0; i < 8; i++) w[i] = L'Z';
    n = _snwprintf(w, 4, L"%s", w6);
    CHECK_EQ_I(n, -1); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[3], 'd'); CHECK_EQ_U(w[4], 'Z');
}
TC_CASE(truncated_S) {
    wchar_t w[8]; int n, i; for (i = 0; i < 8; i++) w[i] = L'Z';
    n = _snwprintf(w, 4, L"%S", n6);
    CHECK_EQ_I(n, -1); CHECK_EQ_U(w[0], 'a'); CHECK_EQ_U(w[3], 'd'); CHECK_EQ_U(w[4], 'Z');
}

int main(void) { tc_begin(); TC_RUN(truncated_s); TC_RUN(truncated_S); return tc_end(); }
