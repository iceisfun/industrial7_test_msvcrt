/* TC
 * id:    pfo_neg_probe_vsprintf_bad_length_modifiers
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf __stdio_common_vsprintf_s
 * crt:   ucrtbase
 * tier:  p1
 * conf:  probe
 * ref:   MS docs printf Size specification
 * note:  OBSERVED Win10 22H2 19045 ucrtbase (run twice): the non-secure worker rejects "%hhhd" (-1, EINVAL, handler) but treats "%I7d" as literal "7d" (return 2) and a dangling "%I64" as empty
 *        output (return 0, no handler). The secure worker rejects all three (-1, EINVAL, handler once). Old expectation -1 for i_7 / i64_end was wrong for the non-secure path.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int c0(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(0, b, n, f, NULL, ap); va_end(ap); return r; }
static int cs(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf_s(0, b, n, f, NULL, ap); va_end(ap); return r; }

TC_CASE(hhh)  { char b[16]; int r; errno = 0; r = c0(b, sizeof b, "%hhhd", 1); CHECK_EQ_I(r, -1); }
TC_CASE(i_7)   { char b[16]; int r; errno = 0; tc_iph_count = 0; r = c0(b, sizeof b, "%I7d", 1); CHECK_EQ_I(r, 2); CHECK_STR(b, "7d"); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(i64_end) { char b[16]; int r; errno = 0; tc_iph_count = 0; b[0] = 'Q'; r = c0(b, sizeof b, "%I64", 1LL); CHECK_EQ_I(r, 0); CHECK_STR(b, ""); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(i64_end_s) { char b[16]; int r; errno = 0; tc_iph_count = 0; r = cs(b, sizeof b, "%I64", 1LL); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(i_7_s) { char b[16]; int r; errno = 0; tc_iph_count = 0; r = cs(b, sizeof b, "%I7d", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(hhh_s) { char b[16]; int r; tc_iph_count = 0; r = cs(b, sizeof b, "%hhhd", 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(hhh); TC_RUN(i_7); TC_RUN(i64_end); TC_RUN(hhh_s); TC_RUN(i64_end_s); TC_RUN(i_7_s);
    return tc_end();
}
