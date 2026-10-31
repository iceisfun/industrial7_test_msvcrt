/* TC
 * id:    pfq_neg_sprintf_p_null_buffer
 * area:  stdio_printf
 * kind:  negative
 * funcs: _sprintf_p _vsprintf_p
 * procs: _set_invalid_parameter_handler
 * crt:   >=90
 * tier:  p0
 * conf:  documented
 * ref:   MS docs sprintf_p, _sprintf_p_l, _vsprintf_p: if buffer or format is NULL the invalid parameter handler is invoked, return -1, errno EINVAL
 * note:  NULL buffer with size 0 is NOT tested here (count-only form is used internally by _scprintf_p); see pfq_neg_probe_sprintf_p_zero_size.
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

static int cv(char *b, size_t n, const char *f, ...)
{ va_list ap; int r; va_start(ap, f); r = pq_vsprintf_p(b, n, f, ap); va_end(ap); return r; }

TC_CASE(sprintf_p_null)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = pq_sprintf_p(b, 10, "%1$d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(vsprintf_p_null)
{
    char * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = cv(b, 10, "%1$d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(swprintf_p_null)
{
    wchar_t * volatile b = NULL; int r;
    tc_iph_count = 0; errno = 0;
    r = pq_swprintf_p(b, 10, L"%1$d", 1);
    CHECK_EQ_I(r, -1);
    CHECK_EQ_I(errno, EINVAL);
    CHECK(tc_iph_count >= 1);
}
TC_CASE(valid_still_works)
{
    char b[16]; int r;
    tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, "%1$d", 1);
    CHECK_EQ_I(r, 1); CHECK_STR(b, "1"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(sprintf_p_null); TC_RUN(vsprintf_p_null); TC_RUN(swprintf_p_null); TC_RUN(valid_still_works);
    return tc_end();
}
