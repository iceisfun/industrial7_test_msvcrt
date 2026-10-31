/* TC
 * id:    ucr_neg_stdio_vsnprintf_s_count0_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsnprintf_s
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s
 * note:  PROBES: count 0 with size 4 and output "abcdef". Asserted: return is 0 or -1 (count 0 with output longer than count may be treated as truncation, like count 3 in
 *        the documented sibling), and no handler call
 *        (nothing is written, so the buffer contents are not asserted beyond not exceeding the size).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

typedef int (__cdecl *vsn_t)(unsigned long long, char *, size_t, size_t, const char *, void *, va_list);
static vsn_t p_vsn;
static int calln(char *b, size_t n, size_t c, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p_vsn(0, b, n, c, fmt, NULL, ap); va_end(ap); return r; }

TC_CASE(count0)
{
    char b[8]; int r;
    if (!p_vsn) TC_SKIP("not exported");
    memset(b, 'S', sizeof b); tc_iph_count = 0;
    r = calln(b, 4, 0, "abcdef");
    CHECK(r == 0 || r == -1); CHECK_EQ_I(tc_iph_count, 0); CHECK_EQ_I(b[4], 'S');
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsn = (vsn_t)tc_crt_proc("__stdio_common_vsnprintf_s");
    TC_RUN(count0);
    return tc_end();
}
