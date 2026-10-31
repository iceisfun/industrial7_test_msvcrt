/* TC
 * id:    ucr_neg_stdio_vswprintf_null_count0_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vswprintf
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs vswprintf / _vsnwprintf (return -1 when the output does not fit)
 * note:  PROBES: __stdio_common_vswprintf with options 0 (legacy), NULL buffer and count 0 and output "abc".
 *        Either -1 (legacy: does not fit) or 3 (size query) is plausible; asserted: no crash, ret is -1 or 3.
 *        With the standard-snprintf bit (2) the C11 size query must return 3.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <wchar.h>

typedef int (__cdecl *vws_t)(unsigned long long, wchar_t *, size_t, const wchar_t *, void *, va_list);
static vws_t p_vws;
static int callw(unsigned long long o, wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vws(o, b, n, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(legacy_null_count0)
{
    int r;
    if (!p_vws) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = callw(0, NULL, 0, L"abc");
    CHECK(r == -1 || r == 3);
}
TC_CASE(std_null_count0)
{
    int r;
    if (!p_vws) TC_SKIP("not exported");
    tc_iph_count = 0;
    r = callw(2, NULL, 0, L"abc");
    CHECK_EQ_I(r, 3); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vws = (vws_t)tc_crt_proc("__stdio_common_vswprintf");
    TC_RUN(legacy_null_count0); TC_RUN(std_null_count0);
    return tc_end();
}
