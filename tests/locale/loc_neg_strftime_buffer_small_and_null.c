/* TC
 * id:    loc_neg_strftime_buffer_small_and_null
 * area:  locale
 * funcs: strftime
 * crt:   all
 * tier:  p1
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.27.3.5 (returns 0 if result including NUL exceeds maxsize); MS docs strftime (NULL format: EINVAL, VS2005+)
 * note:  The NULL-format case is asserted only for generations >= 80 (parameter validation).
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static struct tm mk(void)
{
    struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 124; t.tm_mon = 0; t.tm_mday = 7; t.tm_wday = 0; t.tm_yday = 6;
    t.tm_hour = 9; t.tm_min = 5; t.tm_sec = 3;
    return t;
}
static const char * volatile v_f;
static const char * volatile v_null;
static volatile size_t v_n;

TC_CASE(too_small_returns_0)
{
    char b[16]; struct tm t = mk();
    v_f = "%A"; v_n = 3;
    CHECK_EQ_U(strftime(b, v_n, v_f, &t), 0);
    v_f = "%Y-%m-%d"; v_n = 5;
    CHECK_EQ_U(strftime(b, v_n, v_f, &t), 0);
}
TC_CASE(exact_fit_needs_nul)
{
    char b[16]; struct tm t = mk();
    v_f = "%a";
    v_n = 3; CHECK_EQ_U(strftime(b, v_n, v_f, &t), 0);
    v_n = 4; CHECK_EQ_U(strftime(b, v_n, v_f, &t), 3);
    CHECK_STR(b, "Sun");
}
TC_CASE(null_format_errno_einval_where_supported)
{
    char b[16]; struct tm t = mk();
    if (!TC_AT_LEAST(80)) TC_SKIP("pre-VS2005");
    if (!tc_iph_install()) TC_SKIP("no IPH api");
    tc_iph_count = 0; errno = 0; b[0] = 'x';
    CHECK_EQ_U(strftime(b, sizeof b, v_null, &t), 0);
    CHECK_EQ_I(errno, EINVAL);
    CHECK_EQ_I(tc_iph_count, 1);
}
int main(void)
{
    tc_begin(); tc_iph_install();   /* so a too-small buffer can never hit the fatal default handler */
    TC_RUN(too_small_returns_0); TC_RUN(exact_fit_needs_nul); TC_RUN(null_format_errno_einval_where_supported);
    return tc_end();
}
