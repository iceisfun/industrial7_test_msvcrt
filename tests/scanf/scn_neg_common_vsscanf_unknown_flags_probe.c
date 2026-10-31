/* TC
 * id:    scn_neg_common_vsscanf_unknown_flags_probe
 * area:  scanf
 * kind:  negative
 * funcs:
 * procs: __stdio_common_vsscanf
 * crt:   ucrtbase
 * tier:  p2
 * conf:  probe
 * ref:   (undocumented) __stdio_common_vsscanf options bitmask; defined bits are low bits only
 * note:  PROBE: undefined high option bits (0x80000000, 0xFFFFFFFF, 0x8000000000000000) are probed to be ignored: "%d" on "42"
 *        gives the same result (1, value 42) as options 0. Note 0xFFFFFFFF also sets the defined low bits
 *        (secure/legacy), which do not affect a plain %d.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

typedef int (__cdecl *cn_t)(unsigned long long, const char *, size_t, const char *, void *, va_list);
static cn_t p_cn;
static int call(unsigned long long o, const char *s, const char *f, ...) { va_list ap; int r; va_start(ap, f); r = p_cn(o, s, (size_t)-1, f, NULL, ap); va_end(ap); return r; }
static volatile unsigned long long opt;

static void run(unsigned long long o)
{
    int x = 0, r; if (!p_cn) TC_SKIP("not exported"); tc_iph_install(); opt = o;
    r = call(opt, "42", "%d", &x);
    CHECK_EQ_I(r, 1); CHECK_EQ_I(x, 42);
}
TC_CASE(zero_control) { run(0); }
TC_CASE(bit31) { run(0x80000000ULL); }
TC_CASE(all32) { run(0xFFFFFFFFULL); }
TC_CASE(bit63) { run(0x8000000000000000ULL); }
int main(void)
{
    tc_begin(); p_cn = (cn_t)tc_crt_proc("__stdio_common_vsscanf");
    TC_RUN(zero_control); TC_RUN(bit31); TC_RUN(all32); TC_RUN(bit63);
    return tc_end();
}
