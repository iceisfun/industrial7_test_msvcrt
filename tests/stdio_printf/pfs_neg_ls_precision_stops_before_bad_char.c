/* TC
 * id:    pfs_neg_ls_precision_stops_before_bad_char
 * area:  stdio_printf
 * funcs: sprintf
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   MS docs printf type field s/ls and precision (maximum number of characters printed); UCRT sets EILSEQ when a wide character cannot be converted
 * note:  PROBE: in the C locale U+20AC cannot be converted to a narrow char. Probed: the precision limits how many wide characters are converted, so a bad character beyond the precision is never examined (%.2ls ok, %.3ls fails EILSEQ, %.0ls ok).
 */
#include "tc.h"
#include <stdio.h>
#include <errno.h>

static const wchar_t *volatile ws_bad = L"ab\x20ac";
static const wchar_t *volatile ws_euro = L"\x20ac";
static const char *volatile f_p2 = "%.2ls";
static const char *volatile f_p3 = "%.3ls";
static const char *volatile f_p0 = "%.0ls";

TC_CASE(prec2_ok)
{
    char b[16]; int n; b[0] = 'Z'; errno = 0;
    n = sprintf(b, f_p2, ws_bad);
    CHECK_EQ_I(n, 2); CHECK_STR(b, "ab");
}
TC_CASE(prec3_fails)
{
    char b[16]; int n; errno = 0;
    n = sprintf(b, f_p3, ws_bad);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EILSEQ);
}
TC_CASE(prec0_ok)
{
    char b[16]; int n; b[0] = 'Z'; errno = 0;
    n = sprintf(b, f_p0, ws_euro);
    CHECK_EQ_I(n, 0); CHECK(b[0] == 0);
}
int main(void) { tc_begin(); TC_RUN(prec2_ok); TC_RUN(prec3_fails); TC_RUN(prec0_ok); return tc_end(); }
