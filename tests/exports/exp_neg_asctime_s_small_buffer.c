/* TC
 * id:    exp_neg_asctime_s_small_buffer
 * area:  exports
 * funcs:
 * procs: asctime_s
 * crt:   >=80
 * tier:  p1
 * kind:  negative
 * conf:  documented
 * ref:   MS docs asctime_s: buffer must be >= 26 chars; 0 < size < 26 -> EINVAL (docs return-value table), tm fields out of range -> EINVAL
 * note:  Spec guessed ERANGE for small buffer; MS docs table says EINVAL. The spec named "_asctime_s"; the real export is asctime_s.
 */
#include "tc.h"
#include <time.h>
#include <errno.h>

typedef int (__cdecl *asctime_s_t)(char *, size_t, const struct tm *);
static asctime_s_t p_asctime_s;

static void mk(struct tm *t)
{
    volatile int z = 0;
    t->tm_sec = z; t->tm_min = z; t->tm_hour = z; t->tm_mday = 1; t->tm_mon = z; t->tm_year = 123;
    t->tm_wday = z; t->tm_yday = z; t->tm_isdst = z;
}

TC_CASE(buffer25_einval)
{
    struct tm t; char b[32]; int r;
    if (!p_asctime_s) TC_SKIP("asctime_s not exported");
    mk(&t); tc_iph_install(); tc_iph_count = 0;
    r = p_asctime_s(b, 25, &t);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count > 0);
}
TC_CASE(null_buffer_einval)
{
    struct tm t; int r;
    if (!p_asctime_s) TC_SKIP("asctime_s not exported");
    mk(&t); tc_iph_install(); tc_iph_count = 0;
    r = p_asctime_s(NULL, 26, &t);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count > 0);
}
TC_CASE(bad_month_einval)
{
    struct tm t; char b[32]; int r;
    if (!p_asctime_s) TC_SKIP("asctime_s not exported");
    mk(&t); t.tm_mon = 12; tc_iph_install(); tc_iph_count = 0;
    r = p_asctime_s(b, sizeof b, &t);
    CHECK_EQ_I(r, EINVAL);
    CHECK(tc_iph_count > 0);
}
TC_CASE(valid_26_ok)
{
    struct tm t; char b[26]; int r;
    if (!p_asctime_s) TC_SKIP("asctime_s not exported");
    mk(&t);
    r = p_asctime_s(b, 26, &t);
    CHECK_EQ_I(r, 0);
    CHECK(b[8] == ' ' || b[8] == '0');   /* day padding differs between CRT generations; not asserted */
    CHECK_EQ_I(b[9], '1');
    b[8] = 'x'; b[9] = 'y';
    CHECK_STR(b, "Sun Jan xy 00:00:00 2023\n");
}
int main(void)
{
    tc_begin();
    p_asctime_s = (asctime_s_t)tc_crt_proc("asctime_s");
    TC_RUN(buffer25_einval); TC_RUN(null_buffer_einval); TC_RUN(bad_month_einval); TC_RUN(valid_26_ok);
    return tc_end();
}
