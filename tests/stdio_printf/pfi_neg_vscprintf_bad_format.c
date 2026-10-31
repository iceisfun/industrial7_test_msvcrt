/* TC
 * id:    pfi_neg_vscprintf_bad_format
 * area:  stdio_printf
 * kind:  negative
 * funcs: _vscprintf _scprintf
 * crt:   ucrtbase
 * tier:  p0
 * conf:  documented
 * ref:   MS docs _scprintf, _vscprintf: returns -1 on error; unknown conversion char after % invokes the invalid parameter handler (EINVAL) per printf type-field docs
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <errno.h>

static int vs(const char *f, ...) { int r; va_list ap; va_start(ap, f); r = _vscprintf(f, ap); va_end(ap); return r; }
static const char *volatile F1 = "%d %y", *volatile F3 = "%!d";

TC_CASE(scprintf_bad_conv) { int r; errno = 0; tc_iph_count = 0; r = _scprintf((const char *)F1, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(scprintf_bang) { int r; tc_iph_count = 0; r = _scprintf((const char *)F3, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vscprintf_same)
{
    int r;
    tc_iph_count = 0; r = vs((const char *)F1, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
    tc_iph_count = 0; r = vs((const char *)F3, 1); CHECK_EQ_I(r, -1); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(scprintf_valid_control) { int r; tc_iph_count = 0; r = _scprintf("%d", 12345); CHECK_EQ_I(r, 5); CHECK_EQ_I(tc_iph_count, 0); }
int main(void)
{
    tc_begin(); CHECK(tc_iph_install());
    TC_RUN(scprintf_bad_conv); TC_RUN(scprintf_bang); TC_RUN(vscprintf_same); TC_RUN(scprintf_valid_control);
    return tc_end();
}
