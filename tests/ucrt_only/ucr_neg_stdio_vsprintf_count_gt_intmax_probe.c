/* TC
 * id:    ucr_neg_stdio_vsprintf_count_gt_intmax_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsprintf __stdio_common_vsprintf_s
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs sprintf_s / _snprintf (size_t count semantics)
 * note:  PROBES: buffer sizes above INT_MAX (0x80000001) and (size_t)-1 with a short output into a real 32-byte buffer.
 *        Either failure (-1, errno EINVAL/ERANGE) or success is accepted; on success the result must be correct and
 *        NUL-terminated. Asserted: no crash, consistent result. 64-bit only (size_t > INT_MAX).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

typedef int (__cdecl *vs_t)(unsigned long long, char *, size_t, const char *, void *, va_list);
static vs_t p_vs, p_vss;
static int calls(vs_t p, char *b, size_t n, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = p(0, b, n, fmt, NULL, ap); va_end(ap); return r; }

static void run(vs_t p, size_t n)
{
    char b[32]; int r;
    memset(b, 'S', sizeof b);
    errno = 0; tc_iph_count = 0;
    r = calls(p, b, n, "hi%d", 7);
    if (r >= 0) { CHECK_EQ_I(r, 3); CHECK_STR(b, "hi7"); }
    else CHECK(errno == EINVAL || errno == ERANGE);
}
TC_CASE(s_gt_intmax)
{
    if (!p_vss) TC_SKIP("not exported");
    if (sizeof(size_t) < 8) TC_SKIP("32-bit");
    run(p_vss, (size_t)0x80000001ULL);
}
TC_CASE(legacy_size_max)
{
    if (!p_vs) TC_SKIP("not exported");
    run(p_vs, (size_t)-1);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vs = (vs_t)tc_crt_proc("__stdio_common_vsprintf");
    p_vss = (vs_t)tc_crt_proc("__stdio_common_vsprintf_s");
    TC_RUN(s_gt_intmax); TC_RUN(legacy_size_max);
    return tc_end();
}
