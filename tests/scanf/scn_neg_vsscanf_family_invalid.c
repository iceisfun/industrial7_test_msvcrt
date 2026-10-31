/* TC
 * id:    scn_neg_vsscanf_family_invalid
 * area:  scanf
 * kind:  negative
 * funcs: __stdio_common_vsscanf __stdio_common_vfscanf
 * crt:   ucrtbase
 * tier:  p1
 * conf:  inferred
 * ref:   MS docs vsscanf_s / vfscanf_s / sscanf_s (Parameter validation: NULL stream/format -> invalid parameter handler, EOF or 0, errno EINVAL)
 * note:  vsscanf_s and _vfscanf_s_l are inline in the UCRT headers and forward to __stdio_common_vsscanf/vfscanf with the SECURECRT flag.
 *        Expectation: NULL stream or NULL format returns EOF, errno EINVAL, handler called, x untouched. Return/errno for NULL stream/format follow the common scanf parameter validation (inferred; docs do not state it per function).
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int call_vsscanf_s(const char *in, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = vsscanf_s(in, fmt, ap); va_end(ap); return r; }
static int call_vfscanf_s(FILE *f, const char *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = _vfscanf_s_l(f, fmt, NULL, ap); va_end(ap); return r; }

static const char *volatile vin, *volatile vfmt;
static FILE *volatile vstream;

TC_CASE(vfscanf_s_null_stream)
{
    int r, x = 5;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    r = call_vfscanf_s(vstream, "%d", &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); CHECK_EQ_I(x, 5);
}
TC_CASE(vsscanf_s_null_format)
{
    int r, x = 5;
    tc_iph_install(); tc_iph_count = 0; errno = 0;
    vin = "17";
    r = call_vsscanf_s(vin, vfmt, &x);
    CHECK_EQ_I(r, EOF); CHECK_EQ_I(errno, EINVAL); CHECK(tc_iph_count >= 1); CHECK_EQ_I(x, 5);
}
int main(void)
{
    tc_begin();
    TC_RUN(vfscanf_s_null_stream); TC_RUN(vsscanf_s_null_format);
    return tc_end();
}
