/* TC
 * id:    pfs_neg_probe_sprintf_s_null_format_buffer
 * area:  stdio_printf
 * funcs: sprintf_s swprintf_s
 * crt:   >=100
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s (parameter validation); buffer state on NULL format is not documented
 * note:  PROBE: whether sprintf_s/swprintf_s with a NULL format (valid buffer, size 10) set buffer[0] to 0. Guess: not
 *        modified (format validated before the buffer is touched), so the sentinel 'Z' stays. Real Windows decides.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>
#include <errno.h>

static const char *volatile nf = NULL;
static const wchar_t *volatile nwf = NULL;
TC_CASE(null_format_buffer_state)
{
    char b[10]; int n; b[0] = 'Z'; errno = 0; tc_iph_count = 0;
    n = sprintf_s(b, 10, nf);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(b[0] == 'Z');
}
TC_CASE(wide_null_format_buffer_state)
{
    wchar_t w[10]; int n; w[0] = L'Z'; errno = 0; tc_iph_count = 0;
    n = swprintf_s(w, 10, nwf);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(tc_iph_count, 1);
    CHECK(w[0] == L'Z');
}
int main(void) { tc_begin(); CHECK(tc_iph_install()); TC_RUN(null_format_buffer_state); TC_RUN(wide_null_format_buffer_state); return tc_end(); }
