/* TC
 * id:    pfq_neg_probe_sprintf_p_percent_n_positional_errno
 * area:  stdio_printf
 * funcs: _sprintf_p
 * crt:   >=90
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   MS docs _sprintf_p, printf_s (failure sets errno)
 * note:  PROBE: errno is non-zero after each positional failure class (NULL buffer, NULL format, position gap, buffer too small). The specific value (EINVAL vs ERANGE) is not asserted; only that errno was set and the call returned a negative value.
 */
#include "tc.h"
#include "pfq_shim.h"
#include <errno.h>

static char * volatile Nbuf = NULL;
static const char * volatile Nfmt = NULL;
static const char * volatile Fgap = "%1$d%3$d", * volatile Fbig = "%1$s", * volatile Fok = "%1$d";

TC_CASE(null_buf)
{
    int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(Nbuf, 16, Fok, 1);
    CHECK(r < 0); CHECK(errno != 0);
}
TC_CASE(null_fmt)
{
    char b[16]; int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Nfmt, 1);
    CHECK(r < 0); CHECK(errno != 0);
}
TC_CASE(gap)
{
    char b[16]; int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fgap, 1, 2, 3);
    CHECK(r < 0); CHECK(errno != 0);
}
TC_CASE(too_small)
{
    char b[4]; int r; errno = 0; tc_iph_count = 0;
    r = pq_sprintf_p(b, sizeof b, Fbig, "abcdefgh");
    CHECK(r < 0); CHECK(errno != 0);
}
int main(void)
{
    tc_begin();
    if (!tc_iph_install()) { tc_write("TC FAIL " TC_TEST_ID "::__no iph\n"); return 2; }
    TC_RUN(null_buf); TC_RUN(null_fmt); TC_RUN(gap); TC_RUN(too_small);
    return tc_end();
}
