/* TC
 * id:    pfo_neg_vscprintf_null_and_invalid
 * area:  stdio_printf
 * kind:  negative
 * funcs: __stdio_common_vsprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  observed
 * ref:   MS docs _scprintf, _vscprintf (NULL format -> invalid parameter handler, -1, EINVAL); MS docs printf (invalid type character -> invalid parameter); _set_printf_count_output
 * note:  OBSERVED (Win10 22H2 x64 (19045) ucrtbase.dll observed, raw: "bad_type r == -1 got=1"; zzp_probe2 "vscprintf %y: r=1 errno=0 iph=0"): a size query (options 2, NULL buffer)
 *        with an unknown conversion "%y" does NOT validate it: it counts the literal "y" (r=1), no errno, no handler. Cases null_format (NULL
 *        format -> -1/EINVAL/handler), percent_n_disabled and valid_query remain as originally asserted and pass. C11 7.21.6.1p9: UB. Stable.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int c2(const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = __stdio_common_vsprintf(2, NULL, 0, f, NULL, ap); va_end(ap); return r; }

TC_CASE(null_format) { const char *volatile f = NULL; int r; tc_iph_count = 0; errno = 0; r = c2(f);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(bad_type) { int r; tc_iph_count = 0; errno = 0; r = c2("%y", 1);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(errno, 0); CHECK_EQ_I(tc_iph_count, 0); }
TC_CASE(percent_n_disabled) { int t = 77, r; tc_iph_count = 0; r = c2("ab%n", &t);
    CHECK_EQ_I(r, -1); CHECK_EQ_I(t, 77); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(valid_query) { int r; tc_iph_count = 0; r = c2("abc%d", 12); CHECK_EQ_I(r, 5); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) return 2;
    TC_RUN(null_format); TC_RUN(bad_type); TC_RUN(percent_n_disabled); TC_RUN(valid_query);
    return tc_end();
}
