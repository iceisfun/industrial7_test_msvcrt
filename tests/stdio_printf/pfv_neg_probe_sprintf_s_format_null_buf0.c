/* TC
 * id:    pfv_neg_probe_sprintf_s_format_null_buf0
 * area:  stdio_printf
 * kind:  negative
 * funcs: sprintf_s _snprintf_s
 * crt:   ucrtbase
 * tier:  p2
 * conf:  observed
 * ref:   MS docs sprintf_s (on error the buffer is set to an empty string where possible)
 * note:  After a failure caused by a bad format (unknown conversion, %n) the secure functions leave b[0]==0 (unchanged, passes); with a NULL format
 *        the buffer is left UNTOUCHED (b[0] stays 'Z'; observed Win10 22H2, raw: b[0] == 0 got=90). The failing return value is also probed as -1. Buffer is pre-filled with 'Z'.
 */
#include "tc.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

static const char *volatile f_y = "%y";
static const char *volatile f_null = NULL;
static const char *volatile f_n = "ab%n";
static void fill(char *b) { memset(b, 'Z', 16); b[15] = 0; }

TC_CASE(sprintf_s_bad_fmt_buf0) { char b[16]; int r; fill(b); r = sprintf_s(b, sizeof b, f_y); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); }
TC_CASE(sprintf_s_null_fmt_buf0) { char b[16]; int r; fill(b); r = sprintf_s(b, sizeof b, f_null); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 'Z'); }
TC_CASE(sprintf_s_percent_n_buf0) { char b[16]; int n = 0; int r; fill(b); r = sprintf_s(b, sizeof b, f_n, &n); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); }
TC_CASE(snprintf_s_bad_fmt_buf0) { char b[16]; int r; fill(b); r = _snprintf_s(b, sizeof b, 8, f_y); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 0); }
TC_CASE(snprintf_s_null_fmt_buf0) { char b[16]; int r; fill(b); r = _snprintf_s(b, sizeof b, 8, f_null); CHECK_EQ_I(r, -1); CHECK_EQ_I(b[0], 'Z'); }

int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(sprintf_s_bad_fmt_buf0); TC_RUN(sprintf_s_null_fmt_buf0); TC_RUN(sprintf_s_percent_n_buf0);
    TC_RUN(snprintf_s_bad_fmt_buf0); TC_RUN(snprintf_s_null_fmt_buf0);
    return tc_end();
}
