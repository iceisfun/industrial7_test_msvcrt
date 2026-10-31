/* TC
 * id:    tm_neg_strftime_c99_old_probe
 * area:  time
 * kind:  negative
 * funcs: strftime
 * crt:   msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs strftime (C99 conversions only documented for VS2015+ / UCRT)
 * note:  PROBES whether pre-2015 CRTs reject C99 conversions %F %T %D %e %R. Expected (most likely): return 0, errno EINVAL, invalid-parameter handler called. Each specifier is its own case.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

static int probe(const char *f, size_t *rc)
{
    char buf[64]; volatile const char *vf = f; struct tm t; char *p = (char *)&t; size_t i;
    for (i = 0; i < sizeof t; i++) p[i] = 0;
    t.tm_year = 117; t.tm_mon = 2; t.tm_mday = 5; t.tm_hour = 7; t.tm_min = 8; t.tm_sec = 9; t.tm_yday = 63;
    errno = 0; tc_iph_count = 0;
    *rc = strftime(buf, sizeof buf, (const char *)vf, &t);
    return errno;
}
static void expect_reject(const char *f)
{
    size_t rc; int e = probe(f, &rc);
    CHECK_EQ_U(rc, 0); CHECK_EQ_I(e, EINVAL); CHECK_EQ_I(tc_iph_count, 1);
}
TC_CASE(F) { expect_reject("%F"); }
TC_CASE(T) { expect_reject("%T"); }
TC_CASE(D) { expect_reject("%D"); }
TC_CASE(e) { expect_reject("%e"); }
TC_CASE(R) { expect_reject("%R"); }
int main(void)
{
    tc_begin(); tc_iph_install();
    TC_RUN(F); TC_RUN(T); TC_RUN(D); TC_RUN(e); TC_RUN(R);
    return tc_end();
}
