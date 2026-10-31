/* TC
 * id:    cv_neg_strtol_overflow_endptr_wide_digits
 * area:  conv
 * funcs: strtol strtoul strtoull strtoll wcstol
 * crt:   ucrtbase
 * tier:  p0
 * kind:  negative
 * conf:  standard
 * ref:   C11 7.22.1.4 (overflow: LONG_MAX/LONG_MIN/ULONG_MAX, ERANGE; whole subject sequence consumed); MS docs strtol
 */
#include "tc.h"
#include <stdlib.h>
#include <wchar.h>
#include <errno.h>
#include <limits.h>

static char nines[256], mnines[256], zeros5[256];
static wchar_t wnines[256];
static const char *volatile vs;
static const wchar_t *volatile vw;

static void init(void)
{
    int i;
    for (i = 0; i < 200; i++) { nines[i] = '9'; wnines[i] = L'9'; }
    nines[200] = 0; wnines[200] = 0;
    mnines[0] = '-'; for (i = 0; i < 200; i++) mnines[i + 1] = '9';
    mnines[201] = 0;
    for (i = 0; i < 200; i++) zeros5[i] = '0';
    zeros5[200] = '5'; zeros5[201] = 0;
}
TC_CASE(long_max) { char *e; errno = 0; vs = nines; CHECK_EQ_I(strtol(vs, &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 200); }
TC_CASE(long_min) { char *e; errno = 0; vs = mnines; CHECK_EQ_I(strtol(vs, &e, 10), LONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 201); }
TC_CASE(ulong_max) { char *e; errno = 0; vs = nines; CHECK_EQ_U(strtoul(vs, &e, 10), ULONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 200); }
TC_CASE(ll_variants)
{
    char *e;
    errno = 0; vs = nines; CHECK_EQ_I(strtoll(vs, &e, 10), LLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 200);
    errno = 0; vs = mnines; CHECK_EQ_I(strtoll(vs, &e, 10), LLONG_MIN); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 201);
    errno = 0; vs = nines; CHECK_EQ_U(strtoull(vs, &e, 10), ULLONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vs + 200);
}
TC_CASE(leading_zeros_ok) { char *e; errno = 0; vs = zeros5; CHECK_EQ_U(strtoull(vs, &e, 10), 5); CHECK_EQ_I(errno, 0); CHECK_EQ_P(e, vs + 201); }
TC_CASE(wide_long_max) { wchar_t *e; errno = 0; vw = wnines; CHECK_EQ_I(wcstol(vw, &e, 10), LONG_MAX); CHECK_EQ_I(errno, ERANGE); CHECK_EQ_P(e, vw + 200); }
int main(void)
{
    tc_begin(); init();
    TC_RUN(long_max); TC_RUN(long_min); TC_RUN(ulong_max); TC_RUN(ll_variants); TC_RUN(leading_zeros_ok); TC_RUN(wide_long_max);
    return tc_end();
}
