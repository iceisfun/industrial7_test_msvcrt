/* TC
 * id:    ucr_neg_stdio_vsnprintf_s_count_equals_size_probe
 * area:  ucrt_only
 * funcs:
 * procs: __stdio_common_vsnprintf_s
 * crt:   win10
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _snprintf_s
 * note:  PROBE: count == buffer size with a longer string: the count-limited output (4 chars) plus the terminating NUL
 *        does not fit in the 4-byte buffer, so expected -1, ERANGE, one invalid-parameter call, buffer[0]==0.
 *        The docs describe this only for count > size / count == _TRUNCATE, not this exact boundary.
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
TC_CASE(count_equals_size)
{
    char b[4]; int r;
    if (!p_vsns) TC_SKIP("not exported");
    b[0] = 'X'; errno = 0; tc_iph_count = 0;
    r = call(b, 4, 4, "%s", "abcdefgh");
    CHECK_EQ_I(r, -1); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_I(tc_iph_count, 1); CHECK_EQ_I(b[0], 0);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    p_vsns = (vsns_t)tc_crt_proc("__stdio_common_vsnprintf_s");
    TC_RUN(exported); TC_RUN(count_equals_size);
    return tc_end();
}
