/* TC
 * id:    gtm_neg_strftime_internal_small_buffer
 * area:  time
 * funcs: free
 * procs: _Strftime _Gettnames
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p0
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.27.3.5p? strftime (no more than maxsize characters are written); MS docs strftime
 * note:  Overflow behaviour of the internal _Strftime. Guard-byte cases (never write past size) follow the public strftime contract.
 *        Cases labelled *_probe measure buffer[0]==0 and errno==ERANGE after overflow, which are guesses. The invalid-parameter handler is
 *        installed because size 0 may be treated as an invalid parameter.
 */
#include "tc.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

typedef void *(__cdecl *gett_t)(void);
typedef uintptr_t (__cdecl *sf_t)(char *, size_t, const char *, const struct tm *, void *);
static gett_t pg; static sf_t ps;

static struct tm mk(void)
{
    struct tm t; memset(&t, 0, sizeof t);
    t.tm_year = 100; t.tm_mday = 2; t.tm_yday = 1; t.tm_hour = 3; t.tm_min = 4; t.tm_sec = 5;
    return t;
}
static int guard_ok(const char *b, size_t from, size_t to)
{ size_t i; for (i = from; i < to; i++) if (b[i] != 'G') return 0; return 1; }

TC_CASE(size4_no_overrun)
{
    char b[32]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg();
    (void)ps(b, 4, "%Y-%m-%d", &t, lc);
    CHECK(guard_ok(b, 4, sizeof b));
    free(lc);
}
TC_CASE(size4_probe_empty)
{
    char b[32]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg(); errno = 0;
    (void)ps(b, 4, "%Y-%m-%d", &t, lc);
    CHECK_EQ_I(b[0], 0);
    free(lc);
}
TC_CASE(size4_probe_erange)
{
    char b[32]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg(); errno = 0;
    (void)ps(b, 4, "%Y-%m-%d", &t, lc);
    CHECK_EQ_I(errno, ERANGE);
    free(lc);
}
TC_CASE(size0_no_write)
{
    char b[16]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg();
    (void)ps(b, 0, "%Y-%m-%d", &t, lc);
    CHECK(guard_ok(b, 0, sizeof b));
    free(lc);
}
TC_CASE(exact_fit_minus_one_no_overrun)
{
    char b[32]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg();
    (void)ps(b, 10, "%Y-%m-%d", &t, lc);   /* needs 11 */
    CHECK(guard_ok(b, 10, sizeof b));
    free(lc);
}
TC_CASE(no_handler_probe)
{
    char b[32]; struct tm t = mk(); void *lc; if (!pg || !ps) TC_SKIP("missing");
    memset(b, 'G', sizeof b); lc = pg(); tc_iph_count = 0;
    (void)ps(b, 4, "%Y-%m-%d", &t, lc);
    CHECK_EQ_I(tc_iph_count, 0);
    free(lc);
}
int main(void)
{
    tc_begin(); tc_iph_install();
    pg = (gett_t)tc_crt_proc("_Gettnames"); ps = (sf_t)tc_crt_proc("_Strftime");
    TC_RUN(size4_no_overrun); TC_RUN(size4_probe_empty); TC_RUN(size4_probe_erange); TC_RUN(size0_no_write);
    TC_RUN(exact_fit_minus_one_no_overrun); TC_RUN(no_handler_probe);
    return tc_end();
}
