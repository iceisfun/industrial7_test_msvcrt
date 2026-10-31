/* TC
 * id:    ucr_neg_stdio_vsnprintf_s_count_too_big
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsnprintf_s
 * crt:   win10
 * tier:  p0
 * kind:  negative
 * conf:  documented
 * ref:   MS docs _snprintf_s, _snprintf_s_l, _snwprintf_s (buffer too small without _TRUNCATE -> invalid parameter, ERANGE, -1)
 */
#include "tc.h"
#include <stdarg.h>
#include <errno.h>

typedef int (__cdecl *vsns_t)(unsigned long long, char *, size_t, size_t, const char *, void *, va_list);
static vsns_t p_vsns;

static int call(char *b, size_t n, size_t cnt, const char *f, ...)
{
    va_list ap; int r;
    va_start(ap, f); r = p_vsns(0, b, n, cnt, f, NULL, ap); va_end(ap);
    return r;
}

TC_CASE(exported) { CHECK(p_vsns != NULL); }
TC_CASE(count_larger_than_size)
{
    char b[4]; int r;
    if (!p_vsns) TC_SKIP("not exported");
    b[0] = 'X'; errno = 0; tc_iph_count = 0;
    r = call(b, 4, 10, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
TC_CASE(fits_succeeds)
{
    char b[4]; int r;
    if (!p_vsns) TC_SKIP("not exported");
    errno = 0; tc_iph_count = 0;
    r = call(b, 4, 10, "%s", "abc");
    CHECK_EQ_I(r, 3); CHECK_STR(b, "abc"); CHECK_EQ_I(tc_iph_count, 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsns = (vsns_t)tc_crt_proc("__stdio_common_vsnprintf_s");
    TC_RUN(exported); TC_RUN(count_larger_than_size); TC_RUN(fits_succeeds);
    return tc_end();
}
