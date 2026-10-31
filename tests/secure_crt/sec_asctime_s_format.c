/* TC
 * id:    sec_asctime_s_format
 * area:  secure_crt
 * funcs:
 * procs: asctime_s _gmtime64_s _ctime64_s
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  documented
 * ref:   MS docs asctime_s: 26-character fixed-format string "Www Mmm dd hh:mm:ss yyyy\n"; buffer must hold at least 26 chars
 */
#include "tc.h"
#include <time.h>

typedef int (__cdecl *asctime_s_t)(char *, size_t, const struct tm *);
typedef int (__cdecl *gm_t)(struct tm *, const __int64 *);
typedef int (__cdecl *ctime64_s_t)(char *, size_t, const __int64 *);
static asctime_s_t p_asc; static gm_t p_gm; static ctime64_s_t p_ctime;
static size_t slen(const char *s) { size_t n = 0; while (s[n]) n++; return n; }

TC_CASE(format_Tue_Feb_29_00_00_00_2000_nl)
{
    struct tm t; char b[64]; volatile __int64 v = 951782400LL;
    CHECK_EQ_I(p_gm(&t, (const __int64 *)&v), 0);
    CHECK_EQ_I(p_asc(b, sizeof b, &t), 0);
    CHECK_STR(b, "Tue Feb 29 00:00:00 2000\n");
}
TC_CASE(buffer_exactly_26)
{
    struct tm t; char b[26]; volatile __int64 v = 951782400LL + 3600 * 13 + 61;
    CHECK_EQ_I(p_gm(&t, (const __int64 *)&v), 0);
    CHECK_EQ_I(p_asc(b, 26, &t), 0);
    CHECK_STR(b, "Tue Feb 29 13:01:01 2000\n");
    CHECK_EQ_U(slen(b), 25);
}
TC_CASE(ctime64_s_length_25_plus_nl)
{
    char b[64]; volatile __int64 v = 86400LL * 12000 + 43200; int r;
    if (!p_ctime) TC_SKIP("_ctime64_s not exported");
    r = p_ctime(b, sizeof b, (const __int64 *)&v);
    CHECK_EQ_I(r, 0);
    CHECK_EQ_U(slen(b), 25);
    CHECK_EQ_I(b[24], '\n');
}
int main(void)
{
    tc_begin();
    p_asc = (asctime_s_t)tc_crt_proc("asctime_s"); p_gm = (gm_t)tc_crt_proc("_gmtime64_s"); p_ctime = (ctime64_s_t)tc_crt_proc("_ctime64_s");
    CHECK(p_asc && p_gm); if (!(p_asc && p_gm)) return tc_end();
    TC_RUN(format_Tue_Feb_29_00_00_00_2000_nl); TC_RUN(buffer_exactly_26); TC_RUN(ctime64_s_length_25_plus_nl);
    return tc_end();
}
