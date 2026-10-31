/* TC
 * id:    env_neg_probe_putenv_s_oversize
 * area:  env
 * kind:  negative
 * funcs: _putenv_s getenv _putenv malloc free
 * crt:   ucrtbase msvcr120 msvcr110 msvcr100 msvcr90
 * tier:  p1
 * conf:  probe
 * ref:   MS docs _putenv_s; Win32 environment variable limit is 32767 chars
 * note:  PROBE: value of 40000 chars exceeds the documented Win32 per-variable limit. Only internal consistency is asserted:
 *        no crash, return code 0 iff the lookup returns the full value, and an existing variable is either intact or
 *        replaced, never lost/garbled. Does not assume whether Windows accepts or rejects it.
 */
#include "tc.h"
#include <stdlib.h>
#include <errno.h>

#define BIGN 40000

TC_CASE(oversize_no_crash_consistent)
{
    char *v = (char *)malloc(BIGN + 1); size_t i, len = 0; errno_t r; const char *g;
    if (!v) TC_SKIP("malloc");
    for (i = 0; i < BIGN; i++) v[i] = 'x';
    v[BIGN] = 0;
    errno = 0;
    r = _putenv_s("TC_BIG2", v);
    g = getenv("TC_BIG2");
    if (g) while (g[len]) len++;
    if (r == 0) { CHECK(g != NULL); CHECK_EQ_U(len, BIGN); }
    else CHECK(g == NULL);
    _putenv("TC_BIG2=");
    free(v);
}
TC_CASE(oversize_does_not_corrupt_existing)
{
    char *v = (char *)malloc(BIGN + 1); size_t i, len = 0; errno_t r; const char *g;
    if (!v) TC_SKIP("malloc");
    for (i = 0; i < BIGN; i++) v[i] = 'y';
    v[BIGN] = 0;
    _putenv("TC_BIG=orig");
    r = _putenv_s("TC_BIG", v);
    g = getenv("TC_BIG");
    CHECK(g != NULL);
    if (g) while (g[len]) len++;
    if (r == 0) CHECK_EQ_U(len, BIGN);
    else CHECK_STR(g, "orig");
    _putenv("TC_BIG=");
    CHECK(getenv("TC_BIG") == NULL);
    free(v);
}
int main(void)
{
    tc_begin();
    tc_iph_install();
    TC_RUN(oversize_no_crash_consistent); TC_RUN(oversize_does_not_corrupt_existing);
    return tc_end();
}
