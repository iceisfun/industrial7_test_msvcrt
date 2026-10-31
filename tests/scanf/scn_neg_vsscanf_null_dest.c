/* TC
 * id:    scn_neg_vsscanf_null_dest
 * area:  scanf
 * kind:  negative
 * funcs: __stdio_common_vsscanf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs vsscanf_s / vfscanf_s / sscanf_s (Parameter validation: NULL buffer -> invalid parameter handler, EOF or 0, errno EINVAL)
 * note:  vsscanf_s and _vfscanf_s_l are inline in the UCRT headers and forward to __stdio_common_vsscanf/vfscanf with the SECURECRT flag.
 *        Expectation: NULL %s destination calls the invalid parameter handler and does not return 1. Isolated because an implementation may crash. Handler-called is inferred from the secure-CRT null-pointer validation; the exact return value is deliberately not asserted beyond != 1.
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>

static int call_vsscanf_s(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vsscanf_s(in, fmt, ap); va_end(ap); return r; }
static const char *volatile vin;

TC_CASE(vsscanf_s_null_dest)
{
    int r; char *volatile d = NULL;
    tc_iph_install(); tc_iph_count = 0;
    vin = "abc";
    r = call_vsscanf_s(vin, "%s", d, (unsigned)8);
    CHECK(tc_iph_count >= 1);
    CHECK(r != 1);
}
int main(void)
{
    tc_begin();
    TC_RUN(vsscanf_s_null_dest);
    return tc_end();
}
