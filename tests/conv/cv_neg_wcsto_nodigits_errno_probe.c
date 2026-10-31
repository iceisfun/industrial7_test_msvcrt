/* TC
 * id:    cv_neg_wcsto_nodigits_errno_probe
 * area:  conv
 * funcs: wcstod wcstol wcstoul _wcstoi64
 * crt:   ucrtbase
 * tier:  p1
 * kind:  negative
 * conf:  probe
 * ref:   C11 7.22.1.4 (errno may be set to EINVAL on no conversion; not required)
 * note:  PROBES: that wide strto* functions do NOT touch errno when nothing is converted (the narrow ucrt functions are believed not to set EINVAL).
 */
#include "tc.h"
#include <wchar.h>
#include <stdlib.h>
#include <errno.h>

static const wchar_t *volatile v_s;
#define SENT 12345

TC_CASE(wcstod_errno)
{
    wchar_t *e; v_s = L"abc"; errno = SENT; wcstod(v_s, &e); CHECK_EQ_I(errno, SENT);
    v_s = L""; errno = SENT; wcstod(v_s, &e); CHECK_EQ_I(errno, SENT);
}
TC_CASE(wcstol_errno)
{
    wchar_t *e; v_s = L"xyz"; errno = SENT; wcstol(v_s, &e, 10); CHECK_EQ_I(errno, SENT);
    v_s = L"-"; errno = SENT; wcstol(v_s, &e, 10); CHECK_EQ_I(errno, SENT);
}
TC_CASE(wcstoul_errno)
{
    wchar_t *e; v_s = L"xyz"; errno = SENT; wcstoul(v_s, &e, 10); CHECK_EQ_I(errno, SENT);
}
TC_CASE(wcstoi64_errno)
{
    wchar_t *e; v_s = L"   "; errno = SENT; _wcstoi64(v_s, &e, 10); CHECK_EQ_I(errno, SENT);
}

int main(void)
{
    tc_begin();
    TC_RUN(wcstod_errno); TC_RUN(wcstol_errno); TC_RUN(wcstoul_errno); TC_RUN(wcstoi64_errno);
    return tc_end();
}
