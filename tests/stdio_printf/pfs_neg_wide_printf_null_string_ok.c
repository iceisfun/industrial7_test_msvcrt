/* TC
 * id:    pfs_neg_wide_printf_null_string_ok
 * area:  stdio_printf
 * kind:  negative
 * funcs: _snwprintf
 * crt:   >=80
 * tier:  p1
 * conf:  observed
 * ref:   MS docs printf type field (s); classic CRT "(null)" substitution
 * note:  Guard: a NULL %s argument is NOT an invalid parameter; the CRT prints "(null)" and the handler must not run.
 *        Observed behaviour, not documented, hence separate from the documented NULL format/buffer tests.
 */
#include "tc.h"
#include <stdio.h>
#include <wchar.h>

static const wchar_t * volatile nullstr = NULL;
static const wchar_t * volatile sfmt = L"%s";

TC_CASE(null_string_arg_ok)
{
    wchar_t w[16]; int r;
    CHECK(tc_iph_install());
    tc_iph_count = 0;
    r = _snwprintf(w, 16, sfmt, nullstr);
    CHECK_EQ_I(r, 6);
    CHECK_WSTR(w, L"(null)");
    CHECK_EQ_I(tc_iph_count, 0);
}
int main(void) { tc_begin(); TC_RUN(null_string_arg_ok); return tc_end(); }
