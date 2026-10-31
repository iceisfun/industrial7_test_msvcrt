/* TC
 * id:    exp_secure_time_exported
 * area:  exports
 * funcs:
 * procs: _localtime64_s _gmtime64_s asctime_s _ctime64_s _wasctime_s _wctime64_s _strdate_s _strtime_s _wstrdate_s _wstrtime_s
 * crt:   >=80
 * tier:  p1
 * conf:  documented
 * ref:   MS docs _localtime64_s, _gmtime64_s, asctime_s, _ctime64_s, _strdate_s
 * note:  The exported name is asctime_s (no leading underscore); the wide form is _wasctime_s.
 */
#include "tc.h"
#include <time.h>
static void need(const char *const *n)
{
    for (; *n; n++) if (!tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("missing export: "); tc_write(*n); tc_write("\n"); }
}
static void absent(const char *const *n)
{
    for (; *n; n++) if (tc_crt_proc(*n)) { tc_failhdr(__FILE__, __LINE__); tc_write("unexpected export: "); tc_write(*n); tc_write("\n"); }
}

TC_CASE(exported_all)
{
    static const char *const n[] = { "_localtime64_s", "_gmtime64_s", "asctime_s", "_ctime64_s", "_wasctime_s", "_wctime64_s",
        "_strdate_s", "_strtime_s", "_wstrdate_s", "_wstrtime_s", 0 };
    need(n);
}
TC_CASE(localtime_gmtime_s)
{
    int (__cdecl *gm)(struct tm *, const long long *) = (int (__cdecl *)(struct tm *, const long long *))tc_crt_proc("_gmtime64_s");
    int (__cdecl *lt)(struct tm *, const long long *) = (int (__cdecl *)(struct tm *, const long long *))tc_crt_proc("_localtime64_s");
    struct tm t; long long z = 86400LL * 365;   /* 1971-01-01 00:00:00 UTC */
    if (!gm || !lt) TC_SKIP("not exported");
    CHECK_EQ_I(gm(&t, &z), 0);
    CHECK_EQ_I(t.tm_year, 71); CHECK_EQ_I(t.tm_mon, 0); CHECK_EQ_I(t.tm_mday, 1); CHECK_EQ_I(t.tm_hour, 0); CHECK_EQ_I(t.tm_wday, 5); CHECK_EQ_I(t.tm_yday, 0);
    CHECK_EQ_I(lt(&t, &z), 0);   /* any time zone: result is valid, only the return code is asserted */
}
TC_CASE(asctime_ctime_s)
{
    int (__cdecl *gm)(struct tm *, const long long *) = (int (__cdecl *)(struct tm *, const long long *))tc_crt_proc("_gmtime64_s");
    int (__cdecl *as)(char *, size_t, const struct tm *) = (int (__cdecl *)(char *, size_t, const struct tm *))tc_crt_proc("asctime_s");
    struct tm t; long long z = 86400LL * 14; char b[32];
    if (!gm || !as) TC_SKIP("not exported");
    CHECK_EQ_I(gm(&t, &z), 0);
    CHECK_EQ_I(as(b, sizeof b, &t), 0); CHECK_STR(b, "Thu Jan 15 00:00:00 1970\n");
}
TC_CASE(strdate_strtime_s)
{
    int (__cdecl *d)(char *, size_t) = (int (__cdecl *)(char *, size_t))tc_crt_proc("_strdate_s");
    int (__cdecl *t)(char *, size_t) = (int (__cdecl *)(char *, size_t))tc_crt_proc("_strtime_s");
    char b[16]; int i;
    if (!d || !t) TC_SKIP("not exported");
    CHECK_EQ_I(d(b, sizeof b), 0); CHECK_EQ_I(b[8], 0); CHECK_EQ_I(b[2], '/'); CHECK_EQ_I(b[5], '/');   /* mm/dd/yy */
    for (i = 0; i < 16; i++) b[i] = 0;
    CHECK_EQ_I(t(b, sizeof b), 0); CHECK_EQ_I(b[8], 0); CHECK_EQ_I(b[2], ':'); CHECK_EQ_I(b[5], ':');    /* hh:mm:ss */
}
int main(void) { tc_begin(); TC_RUN(exported_all); TC_RUN(localtime_gmtime_s); TC_RUN(asctime_ctime_s); TC_RUN(strdate_strtime_s); return tc_end(); }
