/* TC
 * id:    gtm_strftime_internal_return_probe
 * area:  time
 * funcs: free
 * procs: _Strftime _Gettnames
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  positive
 * conf:  probe
 * ref:   undocumented yvals/xtime helper _Strftime
 * note:  PROBE: return value convention of _Strftime. The result is printed as a TC NOTE (buf pointer / length / length+1 / other);
 *        the case asserts it is one of buf, strlen, strlen+1. Empty format yields "" and an exact-fit buffer (strlen+1) succeeds.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef void *(__cdecl *gett_t)(void);
typedef uintptr_t (__cdecl *sf_t)(char *, size_t, const char *, const struct tm *, void *);
static gett_t pg; static sf_t ps;

static struct tm mk(void)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mday = 2; t.tm_yday = 1; t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    return t;
}
TC_CASE(return_convention)
{
    char b[64]; struct tm t = mk(); void *lc; uintptr_t r; char n[24];
    if (!pg || !ps) TC_SKIP("missing");
    lc = pg(); r = ps(b, sizeof b, "%Y-%m-%d", &t, lc);
    tc_write("TC NOTE _Strftime return convention: ");
    if (r == (uintptr_t)b) tc_write("buffer pointer\n");
    else if (r == 10) tc_write("length\n");
    else if (r == 11) tc_write("length+1\n");
    else { tc_i64((long long)r, n); tc_write("other "); tc_write(n); tc_write("\n"); }
    CHECK(r == (uintptr_t)b || r == 10 || r == 11);
    free(lc);
}
TC_CASE(content)
{
    char b[64]; struct tm t = mk(); void *lc;
    if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'x', sizeof b); lc = pg(); (void)ps(b, sizeof b, "%Y-%m-%d", &t, lc);
    CHECK_STR(b, "2000-01-02");
    free(lc);
}
TC_CASE(empty_format)
{
    char b[16]; struct tm t = mk(); void *lc; uintptr_t r; char n[24];
    if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'x', sizeof b); lc = pg(); r = ps(b, sizeof b, "", &t, lc);
    tc_i64((long long)r, n); tc_write("TC NOTE _Strftime(\"\") return="); tc_write(n); tc_write("\n");
    CHECK_STR(b, "");
    free(lc);
}
TC_CASE(exact_fit)
{
    char b[16]; struct tm t = mk(); void *lc;
    if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'x', sizeof b); lc = pg();
    (void)ps(b, 11, "%Y-%m-%d", &t, lc);
    CHECK_STR(b, "2000-01-02");
    CHECK_EQ_I(b[11], 'x');
    free(lc);
}
int main(void)
{
    tc_begin();
    pg = (gett_t)tc_crt_proc("_Gettnames"); ps = (sf_t)tc_crt_proc("_Strftime");
    TC_RUN(content); TC_RUN(return_convention); TC_RUN(empty_format); TC_RUN(exact_fit);
    return tc_end();
}
