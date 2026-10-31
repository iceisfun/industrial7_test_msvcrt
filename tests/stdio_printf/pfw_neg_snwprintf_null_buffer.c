/* TC
 * id:    pfw_neg_snwprintf_null_buffer
 * area:  stdio_printf
 * funcs: _snwprintf _vsnwprintf
 * procs: 
 * crt:   >=80
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf/_snwprintf: invalid parameter handler invoked (VS2005+ parameter validation)
 */
#include "tc.h"
#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <errno.h>

static int vs(wchar_t *b, size_t n, const wchar_t *fmt, ...)
{ va_list ap; int r; va_start(ap, fmt); r = _vsnwprintf(b, n, fmt, ap); va_end(ap); return r; }

TC_CASE(ret_m1) { wchar_t *volatile p = NULL; volatile int v = 1; if (!tc_iph_install()) TC_SKIP("no iph"); CHECK_EQ_I(_snwprintf(p, 10, L"%d", v), -1); }
TC_CASE(errno_einval) { wchar_t *volatile p = NULL; volatile int v = 1; if (!tc_iph_install()) TC_SKIP("no iph"); errno = 0; _snwprintf(p, 10, L"%d", v); CHECK_EQ_I(errno, EINVAL); }
TC_CASE(iph_count_1) { wchar_t *volatile p = NULL; volatile int v = 1; if (!tc_iph_install()) TC_SKIP("no iph"); tc_iph_count = 0; _snwprintf(p, 10, L"%d", v); CHECK_EQ_I(tc_iph_count, 1); }
TC_CASE(vsnwprintf_variant)
{
    wchar_t *volatile p = NULL; int n;
    if (!tc_iph_install()) TC_SKIP("no iph");
    tc_iph_count = 0; errno = 0;
    n = vs(p, 10, L"%d", 1);
    CHECK_EQ_I(n, -1); CHECK_EQ_I(errno, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
int main(void) { tc_begin(); TC_RUN(ret_m1); TC_RUN(errno_einval); TC_RUN(iph_count_1); TC_RUN(vsnwprintf_variant); return tc_end(); }
